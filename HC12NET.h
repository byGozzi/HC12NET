/*
  Message format:
  @Master#Slave$Variable%Value!
    Value:
    Master->Slave
        S for askString
        I for askInt
        F for askFloat
      Slave->Master
        char array for answer

  Privete array:
  _MASTER
  _SLAVE
  _VARIABLE
  _VALUE_CHAR


***** Dynamic of Comunication *****

    Costructor:
    HC12NET(int baud, int waitMillis);
    HC12NET(int baud);

  MASTER side
  void beginMaster (char* masterName)   // begin Master side
    populate _MASTER
    begin Serial
  int HC12NET::askInt(char* SLAVE, char* VARIABLE)  // ask for somethig (string, Int or Float)
    populate _SLAVE
    populate _VARIABLE
    populate _VALUE with S, I or F (asking for string, Int or Float)
    sendSerial () with format: @Master#Slave$Variable%I!
    waiting for answer (readSerial) ...
    readSerial populate _SLAVE_REC, _VARIABLE_REC and _VALUE_REC and compare with _MASTER,_SLAVE_VARIABLE and _VALUE
    readSerial return KO (no valid answer or no answer)
    readSerial return OK (answer ok)
    if readSerial return OK _VALUE_CHAR is populated with asked value (SLAVE has populated _VALUE_CHAR)
    function return asked value (Int value)
  byte HC12NET::sendCommand (char* SLAVE, char* COMMAND)
   populate _SLAVE
   populate _VARIABLE with "CMD"
   populate _VALUE with COMMAND
   sendSerial: @ _MASTER # _SLAVE $ "CMD" % COMMAND !
   waiting for answer (readSerial); // Slave run *char HC12NET::checkMasterCommand()
   readSerial return KO (no valid answer or no answer) => return KO  (so can repeat command)
   readSerial return OK (valid answer) => return OK (Slave performed COMMAND and send OK)




  SLAVE side

  byte HC12NET::beginSlave (char* masterName, char* slaveName);
    populate _MASTER, _SLAVE
    begin Serial
  byte HC12NET::checkMasterInt(char* VARIABLE,int VALUE)  // chek for Int question from master
    byte readSerial ()
    if readSerial return KO (no valid question  or no question)
    if readSerial return OK (valid question) Slave popultate _VALUE_CHAR with converted Int VALUE)
    sendSerial() with format: @Master#Slave$Variable%Value!
  char* HC12NET::checkMasterCommand()
    byte readSerial ()
    if readSerial return KO (no valid command  or no command) return "KO"
    if readSerial return OK (valid question) populate  _VALUE_CHAR with "OK" and return OK


  COMMON FUNCTION
  byte HC1NET::readSerial()
    populate _VALUE_CHAR with _VALUE_CHAR_REC if master,slave and variable are the same
    return OK or KO







  Master send a request to all Slaves:

*/
// ###################  HC12NET.h  ###################



#ifndef HC12NET_H
#define HC12NET_H

#define KO 0
#define OK 1
#define _SIZE 10
#include <Arduino.h>


class HC12NET {
public:
  HC12NET(int baud, int waitMillis);
  HC12NET(int baud);
  // -------- Master side ----------
  void beginMaster(const char* masterName);
  char* askString(const char* SLAVE, const char* VARIABLE);
  int askInt(const char* SLAVE, const char* VARIABLE);
  float askFloat(const char* SLAVE, const char* VARIABLE);
  byte sendCommand(const char* SLAVE, const char* COMMAND);

  // ------- Slave side ----------
  void beginSlave(const char* masterName, const char* slaveName);
  byte checkMasterString(const char* VARIABLE,const  char* VALUE);
  byte checkMasterInt(const char* VARIABLE, int VALUE);
  byte checkMasterFloat(const char* VARIABLE, float VALUE);
  char* checkMasterCommand();
private:
  int _baud;  // speed
  int _waitMillis;

  char _MASTER[_SIZE];
  char _SLAVE[_SIZE];
  char _VARIABLE[_SIZE];
  char _VALUE_CHAR[_SIZE];
  byte readSerial();
  void sendSerial();
};
#endif
