// ------------ Slave: answer to  Float Request --------------
// @Master1#Slave1$speed%F!

#include <HC12NET.h>

HC12NET Connection1(9600, 2000);

void setup() {
  Connection1.beginSlave("Master1", "Slave1");
}

void loop() {
  Connection1.checkMasterFloat("speed", 1250.12);
  delay(5000);
}