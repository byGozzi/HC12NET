// ------------ Master: ask for  String --------------
//             @Master1#Slave1$state%ON1!

#include <HC12NET.h>

HC12NET Connection1(9600, 1000);
void setup() {
  Connection1.beginMaster("Master1");
}
void loop() {
  Serial.println(Connection1.askString("Slave1", "state"));
  delay(5000);
}