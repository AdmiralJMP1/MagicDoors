#include "RemoteReceiver.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Scaning");
  RemoteReceiver::init(0, 3, showCode);
}

void loop() {
}

void showCode(unsigned long receivedCode, unsigned int period) {
  Serial.print("Code: ");
  Serial.print(receivedCode);
  Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");
}
