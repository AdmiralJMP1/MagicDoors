// 686 - ворота права(М.Горького 70)
// 3237 -  вротота сверху(М.Горького 70)
#define txPin 8 //пин передатчик
void setup()
{
  pinMode(txPin, OUTPUT);

  Serial.begin(9600); // тут поставьте свою скорость
  Serial.println("Came started");
}

volatile static long lastCode = 0;

void loop() 
{
  for (int cod = 0; cod < 4096; cod++)
  {  
  SendCame4(3237);
  delay(300); // меньше 300 не работает
  Serial.println(cod);
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
