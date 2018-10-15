#include "RemoteSwitch.h"

#define BTNA  177877
#define BTNB 177903
#define BTNC 177885
#define BTND 177886
#define BTNE 177879
#define BTNF 177904
#define PERIOD 263;


void setup()
{
  Serial.begin(9600);
}


void loop()
{
  transmit(BTNA);
  Serial.println("Sended");
  delay(1000);
}


void transmit(unsigned long rcode) {
  unsigned long code = rcode;
  unsigned long period = PERIOD;
  code |= (unsigned long)period << 23;
  code |= 3L << 20; //(|= 3L)
  RemoteSwitch::sendTelegram(code, 9); // 9 - пин радио передатчика
}
