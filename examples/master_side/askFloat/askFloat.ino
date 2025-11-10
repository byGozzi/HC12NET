// --------- Master: askFloat ----------
//           @Master1#Slave1$speed%12.32!

#include <HC12NET.h>

HC12NET Connection1 (9600,2000);
void setup() {
  Connection1.beginMaster("Master1");
}
void loop() {
  Serial.print(Connection1.askFloat("Slave1", "speed"));
  delay (5000);
}
