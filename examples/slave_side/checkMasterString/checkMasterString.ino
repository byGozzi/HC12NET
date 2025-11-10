// ------------ Slave:answer to String request --------------
//      @Master1#Slave1$state%S!
#include<HC12NET.h>

HC12NET Connection1 (9600, 2000);
void setup() {
  Connection1.beginSlave("Master1", "Slave1");
}
void loop() {
Serial.println(Connection1.checkMasterString("state", "OPEN"));
 // digitalWrite(13, Connection1.checkMasterString("state", "OPEN"));
  delay(5000);
}