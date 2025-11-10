// ------------ Slave:answer to Int request --------------
// @Master1#Slave1$speed%I!
#include <HC12NET.h>

HC12NET Connection1(9600, 1000);
void setup() {
  Connection1.beginSlave("Master1", "Slave1");
  pinMode(13, OUTPUT);
}
void loop() {
  int speedValue = random(0, 100);
  digitalWrite(13, Connection1.checkMasterInt("speed", speedValue));
  delay(50);
}