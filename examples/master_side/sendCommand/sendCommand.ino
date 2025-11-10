// ------------ Master: send command --------------
//      @Master1#Slave1$CMD%LED1ON!
#include <HC12NET.h>

HC12NET Connection1 (9600, 1000); // baud,waitMillis
void setup() {
  Connection1.beginMaster("Master1");
}
void loop() {
  Serial.println(Connection1.sendCommand("Slave1", "LED1ON"));
  delay(3000);
}