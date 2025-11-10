// --------- Master: askInt ----------
// @Master1#Slave1$speed%2222!
#include <HC12NET.h>

HC12NET Connection1(9600,3000);
void setup() {
  Connection1.beginMaster("Master1");
}
void loop() {
  Serial.println(Connection1.askInt("Slave1", "speed"));
  delay(5000);
}