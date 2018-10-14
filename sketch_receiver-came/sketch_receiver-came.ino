#define txPin 8 //пин передатчика
#define rxPin 2 //пин приемника

void setup()
{
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);


  Serial.begin(9600); // тут поставьте свою скорость
  Serial.println("Came started");

  // следующие 2 строчки включают перехватчик. если планируется только отправка их можно закоментить
  attachInterrupt(0, grab, CHANGE); // обратите внимание на первый параметр - если у вас другая ардуина, он может быть не 1, а 0!
  interrupts();
}

volatile static long lastCode = 0;

void loop() {
  // это раскомментить и вместо 3693 вписать свой код для шлагбаума
  //SendCame4(3693);
  //delay(5000);
  //return;

  // при получении кода выводим его в терминал
  if (lastCode > 0)
  {
    Serial.println(lastCode & 0xfff);
    lastCode = 0;
  }
}


// функция отправляет код в эфир
void SendCame4(long Code) {
  for (int j = 0; j < 7; j++) { // посылку посылаем 4 раза подряд.
    // время стартового импульса
    digitalWrite(txPin, HIGH);
    delayMicroseconds(320);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(txPin, LOW); // 1
        delayMicroseconds(640);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(320);
      }
      else {
        digitalWrite(txPin, LOW); // 0
        delayMicroseconds(320);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(640);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(11520);
  }
}


// ВЕСЬ КОД НИЖЕ - ПРИЕМ

#define MAX_DELTA 200
// длительность импульсов может плавать в зависимости от заряда батареии - макс допуск 200 мкс
boolean CheckValue(unsigned int base, unsigned int value)
{
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA)) || ((value < base) && ((base - value) < MAX_DELTA)));
}
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
volatile static byte cameCounter = 0; // count of bits stored
volatile static long cameCode = 0; // code itself

void grab() {
  state = digitalRead(rxPin);
  if (state == HIGH)
    lolen = micros() - prevtime;
  else
    hilen = micros() - prevtime;
  prevtime = micros();

  if (state == LOW)
  {
    // последовательность закончилась
    if (CheckValue(320, hilen) && CheckValue(640, lolen)) // valid 1
    {
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValue(640, hilen) && CheckValue(320, lolen)) // valid 0
    {
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else cameCounter = 0;
  } else if (lolen > 1000 &&
             (cameCounter == 12 || cameCounter == 13) &&
             ((cameCode & 0xfff) != 0xfff))
  {
    lastCode = cameCode & 0xfff;
    cameCounter = 0;
    cameCode = 0;
  }
}
