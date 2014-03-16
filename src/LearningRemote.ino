// -*- c -*-
// LearningRemote Arduino sketch
//
// Copyright (c) 2013,2014 Dave Sieh
// See LICENSE.txt for details.

#include <IRremote.h>       // IR remote control library
#include <pspc_support.h>   // PROGMEM library

const int irPin = A3;   // analog input pin 3 (digital 17)
const long NO_KEY = -1; 
const long TIMEOUT = 5000; //max number of milliseconds to wait for a key (5 secs)
const int KEYCOUNT = 7; // the number of key codes supported
  
long  irKeyCodes[ KEYCOUNT]; // this will store raw codes for all keys            

#define KEY_NAME(i) STRING_FROM_TABLE(remoteKeyNames,i)

const char keyForward[] PROGMEM = "Forward";
const char keyBack[] PROGMEM = "Back";
const char keyLeft[] PROGMEM = "Left";
const char keyRight[] PROGMEM = "Right";
const char keyPivotCW[] PROGMEM = "PivotCW";
const char keyPivotCCW[] PROGMEM = "PivotCCW";
const char keyHalt[] PROGMEM = "Halt";

PGM_P const remoteKeyNames[] PROGMEM = {
  keyForward,
  keyBack,
  keyLeft,
  keyRight,
  keyPivotCW,
  keyPivotCCW,
  keyHalt
};

IRrecv irrecv(irPin);     // create the IR receive object
decode_results results;   // ir data goes here  

void setup() {
  Serial.begin(9600);
  while (!Serial);  // only needed for leonardo
  
  irrecv.enableIRIn(); // Start the ir receiver
  learnKeycodes();
  printConstants();
  Serial.println();
  Serial.println(P("Now press the remote keys to verify correct detection"));
}

void loop() {
  long key = getIrKeycode(TIMEOUT);
  if (key!= NO_KEY) {
    int index = findKey(key);
    if (index != NO_KEY) {
      Serial.println(KEY_NAME(index));
    }
  } 
}

/**
   Get remote control codes.
   The key map should be set to zero before calling this.
*/
void learnKeycodes() {
  Serial.println(P("Ready to learn remote codes"));

  for (int i = 0; i < KEYCOUNT;  ) {
    //delay(100);
    Serial.println();
    Serial.print(P("press remote key for "));
    Serial.print(KEY_NAME(i));    
    long key = getIrKeycode(TIMEOUT);

    if (key > 0 ) {
      Serial.println(P(", release key ..."));
      long temp;

      do {          
        temp = getIrKeycode(200);                
      } while( temp == key);

      if (findKey(key) == NO_KEY) {
        Serial.print(P(" -> using code "));
        Serial.print( key );
        Serial.print(P(" for "));
        Serial.println(KEY_NAME(i)); 
        irKeyCodes[i] = key;        
        i++;
      } else {
        Serial.println(P("key already assigned")); 
      }
    } else {
      continue;
    }
  }     

  Serial.println(P("Done\n"));  
}

/**
   wait up to timeout milliseconds for a key
*/
long getIrKeycode(long timeout) {
  flushKeys();
  
  long key = NO_KEY;
  unsigned long startTime = millis();
  while( millis() - startTime < timeout ) {
    if (irrecv.decode(&results)) { 
      key = results.value;
      //Serial.println(key, HEX);
      irrecv.resume(); // Receive the next value   
      if (key != NO_KEY ) {
        break;
      }
    }  
  }
  return key; 
}

/**
   Clear the buffer.
*/
void flushKeys() {
  while (irrecv.decode(&results)) {
    irrecv.resume(); 
  }
 
  results.value = -1;    
}

/**
   Returns the index for the given key code if found.
   Returns NO_KEY if code is not found.
*/
int findKey(long code) {
  for (int i=0; i < KEYCOUNT; i++) {
    if (irKeyCodes[i] == code) {
      return i;     
    }
  } 
  return NO_KEY;
}

void printConstants() {
  int i = 0; 
  Serial.println(P("// IR remote keycodes:")); 
  Serial.print(P("const long IR_MOVE_FORWARD = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";"));
  Serial.print(P("const long IR_MOVE_BACK    = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";"));
  Serial.print(P("const long IR_MOVE_LEFT    = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";"));
  Serial.print(P("const long IR_MOVE_RIGHT   = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";"));
  Serial.print(P("const long IR_PIVOT_CW     = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";")); 
  Serial.print(P("const long IR_PIVOT_CCW    = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";"));
  Serial.print(P("const long IR_HALT         = ")); Serial.print(irKeyCodes[i++]); 
  Serial.println(P(";"));
 
  Serial.println(); Serial.println(P("Copy the above lines to the Remote tab"));
}
