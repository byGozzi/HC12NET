// ######################  HC12NET.cpp ##################
#include "Arduino.h"
#include <HC12NET.h>

// --------------- Constructor ------------------

   HC12NET::HC12NET (int baud, int waitMillis) {
  _waitMillis = waitMillis;
  _baud = baud;
}


 HC12NET::HC12NET (int baud) {
  _waitMillis = 1000;
  _baud = baud;
}
// --------------- Master side ------------------




void HC12NET::beginMaster (char* masterName) {

  strcpy(_MASTER, masterName);
  Serial.begin(_baud);

 
}

char* HC12NET::askString(char* SLAVE, char* VARIABLE) {
  strcpy(_SLAVE, SLAVE);
  strcpy(_VARIABLE, VARIABLE);
  strcpy (_VALUE_CHAR, "S");
  sendSerial();
  // waiting for serial
  long millisOld = millis();  //
  while (Serial.available() <= 0) {
    if (millis() - millisOld >= _waitMillis) {
      strcpy(_VALUE_CHAR, "KO");
      return _VALUE_CHAR;
    }
  }
  if (readSerial() == OK && millis() - millisOld < _waitMillis) return _VALUE_CHAR;
  strcpy(_VALUE_CHAR, "KO");
  return _VALUE_CHAR;


}

int HC12NET::askInt(char* SLAVE, char* VARIABLE)
{
  strcpy(_SLAVE, SLAVE);
  strcpy(_VARIABLE, VARIABLE);
  strcpy (_VALUE_CHAR, "I");
  sendSerial();
  // send request
  long millisOld = millis();  //
  while (Serial.available() <= 0) {
    if (millis() - millisOld >= _waitMillis) {
      strcpy(_VALUE_CHAR, "KO");
      //return 12345;
      return pow(2, 15);
    }
  }
  // Recived answer OK
  if (readSerial() == OK && millis() - millisOld < _waitMillis) return atoi(_VALUE_CHAR) ;
  // Recived aswer KO
  strcpy(_VALUE_CHAR, "KO");
  return pow(2, 15);
}

float HC12NET::askFloat(char* SLAVE, char* VARIABLE)
{
  strcpy(_SLAVE, SLAVE);
  strcpy(_VARIABLE, VARIABLE);
  strcpy (_VALUE_CHAR, "F");
  sendSerial();
  // waiting for serial
  long millisOld = millis();  //
  while (Serial.available() <= 0) {
    if (millis() - millisOld >= _waitMillis) {
      strcpy(_VALUE_CHAR, "KO");
      return pow(2, 31);
    }
  }
  if (readSerial() == OK) return atof(_VALUE_CHAR) ;//atof
  strcpy(_VALUE_CHAR, "KO");
  return pow(2, 31);
}

byte HC12NET::sendCommand (char* SLAVE, char* COMMAND)
{
  strcpy(_SLAVE, SLAVE);
  strcpy(_VARIABLE, "CMD");
  strcpy (_VALUE_CHAR, COMMAND);
  sendSerial();
  // waiting for serial
  long millisOld = millis();  //
  while (Serial.available() <= 0) {
    if (millis() - millisOld >= _waitMillis) {
      strcpy(_VALUE_CHAR, "KO");
      return KO;
    }
  }
  if (readSerial() == OK && strcmp(_VALUE_CHAR, COMMAND) == 0) return OK;
  return KO;
}

// ---------------------------------------------
// --------------- Slave side ------------------
// ---------------------------------------------

void HC12NET::beginSlave (char* masterName,char* slaveName)
{
  strcpy(_SLAVE, slaveName);
 strcpy(_MASTER, masterName);
 Serial.begin(_baud);

}

byte HC12NET::checkMasterString(char* VARIABLE, char* VALUE)
{
  strcpy(_VARIABLE, VARIABLE);
  if (readSerial() == OK)
  {
   
    if (strcmp(_VALUE_CHAR, "KO") == 0) return KO;
    if (strcmp(_VALUE_CHAR, "S") == 0) {
      strcpy (_VALUE_CHAR, VALUE);
      sendSerial();
      return OK;
    }
  }
  return KO;
}

byte HC12NET::checkMasterInt(char* VARIABLE, int VALUE)
{
  strcpy(_VARIABLE, VARIABLE);
  if (readSerial() == OK) {
    if (strcmp(_VALUE_CHAR, "KO") == 0) return KO;
    if (strcmp(_VALUE_CHAR, "I") == 0) {
      itoa(VALUE, _VALUE_CHAR, _SIZE); //convert int  to char array
      sendSerial();
      return OK;
    }
  }
  return KO;
}


byte HC12NET::checkMasterFloat(char* VARIABLE, float VALUE)
{
  strcpy(_VARIABLE, VARIABLE);
  if (readSerial() == OK) {
    if (strcmp(_VALUE_CHAR, "KO") == 0) return KO;
    if (strcmp(_VALUE_CHAR, "F") == 0)  {
      dtostrf(VALUE, _SIZE, 2, _VALUE_CHAR); //convert float to char array
      sendSerial();
      return OK;
    }
  }
  return KO;
}

char* HC12NET::checkMasterCommand ()
{
  strcpy(_VARIABLE, "CMD");
  if (readSerial() == OK)  {
    char _TEMP [_SIZE];
    strcpy(_TEMP, _VALUE_CHAR);
    sendSerial();
    return _VALUE_CHAR;
  }
  if (readSerial() == KO)  {
    strcpy(_VALUE_CHAR, "KO");
    return _VALUE_CHAR;
  }

}




// --------------- Common functions ---------------

// 1) Read available serial;
// 2) Create and populate arrays _MASTER_REC, _SLAVE_REC, _VARIABLE_REC, _VALUE_REC
// for master, slave, variable and value information read on Serial
// 3) Compare arrays with _MASTER, _SLAVE, _VARIABLE (no _VALUE)
// 4) If data are good populate _VALUE array with _VALUE_REC
// 5) Otherwise, if data are wrong, populate _VALUE array with "ERROR"
byte HC12NET::readSerial()
{
  // example

  char _MASTER_REC[10];
  char _SLAVE_REC[10];
  char _VARIABLE_REC[10];
  char _VALUE_CHAR_REC[10];
  strcpy( _MASTER_REC, "");
  strcpy( _SLAVE_REC, "");
  strcpy( _VARIABLE_REC, "");
  strcpy( _VALUE_CHAR_REC, "");


  char state = 'w';  // waiting
  char character = '\0';
  int x = 0;
  while (Serial.available() > 0) {
    delay(5);
    character = Serial.read();
    if (character == '@') {
      state = '@';
      x = 0;
    }
    if (character == '#') {
      _MASTER_REC[x] = '\0';
      state = '#';
      x = 0;
    }
    if (character == '$') {
      _SLAVE_REC[x] = '\0';
      state = '$';
      x = 0;
    }
    if (character == '%') {
      _VARIABLE_REC[x] = '\0';
      state = '%';
      x = 0;
    }
    if (character == '!') {
      _VALUE_CHAR_REC[x] = '\0';
      state = '!';
      x = 0;
    }
    if (character != '@' && state == '@') {
      _MASTER_REC[x] = character;
      x++;
    }
    if (character != '#' && state == '#') {
      _SLAVE_REC[x] = character;
      x++;
    }
    if (character != '$' && state == '$') {
      _VARIABLE_REC[x] = character;
      x++;
    }
    if (character != '%' && state == '%') {
      _VALUE_CHAR_REC[x] = character;
      x++;
    }
  }
  //Serial.print("received:");
  //Serial.print(_MASTER_REC);
  //Serial.print(_SLAVE_REC);
  //Serial.print(_VARIABLE_REC);
  //Serial.println(_VALUE_CHAR_REC);
  delay(5);
  if (strcmp(_MASTER, _MASTER_REC) != 0 || strcmp(_VARIABLE, _VARIABLE_REC) != 0 || strcmp(_SLAVE, _SLAVE_REC) != 0) return KO;
  strcpy(_VALUE_CHAR, _VALUE_CHAR_REC);
  return OK;
}

// send Serial with _MASTER, _SLAVE, _VARIABLE, _VALUE
void HC12NET::sendSerial() {
  Serial.print("@");
  Serial.print(_MASTER);
  Serial.print("#");
  Serial.print(_SLAVE);
  Serial.print("$");
  Serial.print(_VARIABLE);
  Serial.print("%");
  Serial.print(_VALUE_CHAR);
  Serial.print("!");
  Serial.println();//test
}