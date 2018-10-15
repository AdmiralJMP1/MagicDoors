#define txPin 8 //пин передатчик
#define startSignal 320
#define endSignal 11520
#define longSignal 640
#define shortSignal 320

void setup()
{
  pinMode(txPin, OUTPUT);

  Serial.begin(9600); // тут поставьте свою скорость
  Serial.println("Came started");
}

void loop()
{
  for (int cod = 0; cod < 4096; cod++)
  {
    SendCame4(cod);
    delay(300); // меньше 300 не работает
    Serial.println(cod);
    // однократная отправка кода
    // SendCame4();
  }
}


// функция отправляет код в эфир
void SendCame4(long Code) 
{
  for (int j = 0; j < 7; j++) // посылку посылаем 4 раза подряд.
  { 
    // время стартового импульса
    digitalWrite(txPin, HIGH);
    delayMicroseconds(startSignal);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) 
    {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) 
      {
        digitalWrite(txPin, LOW); // 1
        delayMicroseconds(longSignal);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(shortSignal);
      }
      else 
      {
        digitalWrite(txPin, LOW); // 0
        delayMicroseconds(shortSignal);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(longSignal);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(endSignal);
  }
}
