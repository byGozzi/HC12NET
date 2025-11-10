// ------------ Slave:answer to command --------------
//      @Master1#Slave1$CMD%ON1! 
//      @Master1#Slave1$CMD%OFF1!
#include <HC12NET.h>

HC12NET Connection1(9600,1000);

void setup() {
  Connection1.beginSlave("Master1", "Slave1");
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  String command = Connection1.checkMasterCommand();
  if (command == "ON1") digitalWrite(LED_BUILTIN, HIGH);
  if (command == "OFF1") digitalWrite(LED_BUILTIN, LOW);
  //if(Connection1.checkMasterCommand("ON1")==OK) digitalWrite(13, HIGH);
  //if(Connection1.checkMasterCommand("OFF1")==OK) digitalWrite(13, LOW);
  delay(5000);
}