#include "RemoteSwitch.h"

#define BTNON  177877
#define BTNOFF 177903
#define PERIOD 263; 


void setup()
{
  Serial.begin(9600);
}


void loop()
{
transmit(BTNON);
Serial.println("otpravleno");
delay(10);
transmit(BTNOFF);
delay(10);
transmit(177885);
delay(10);
transmit(177886);
delay(10);
transmit(177879);
delay(10);
transmit(177904);
delay(10);

} 


void transmit(unsigned long rcode){
unsigned long code = rcode;
unsigned long period = PERIOD;
code |= (unsigned long)period << 23;
code |= 3L << 20; //(|= 3L) 
RemoteSwitch::sendTelegram(code, 9); // 9 - пин радио передатчика
}
