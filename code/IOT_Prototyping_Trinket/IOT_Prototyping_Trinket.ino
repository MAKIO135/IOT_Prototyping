#include <SoftwareSerial.h>

#define LED 1
#define BTN 2

SoftwareSerial mySerial( 3, 4 ); // RX, TX
String inputString = ""; // a string to hold incoming data
int len = 0;
boolean stringComplete = false;  // whether the string is complete

int state = 0;
int count = 0;

void setup() {
  inputString.reserve( 200 ); // reserve a buffer in memory for 200 chars max
  mySerial.begin( 19200 );
  pinMode( BTN, INPUT_PULLUP );
  pinMode( LED, OUTPUT );
}

void loop() {
  int currentState = digitalRead( BTN );

  while ( mySerial.available() && !stringComplete ) {
    digitalWrite( LED, LOW );
    char inChar = ( char ) mySerial.read();
    if ( inChar != '\n' ) {
      inputString += inChar;
      len ++;
    }
    else {
      stringComplete = true;
    }
  }

  if ( stringComplete ) {
    mySerial.print( "{count:" );
    mySerial.print( count );
    mySerial.print( "|millis:" );
    mySerial.print( millis() );
    mySerial.println( "}" );
    digitalWrite( LED, HIGH );
    stringComplete = false;
  }
  
  if ( currentState == HIGH ) {
    digitalWrite( LED, LOW );
    state = HIGH;
  }
  else if ( currentState == LOW && state != currentState ) {
    digitalWrite( LED, HIGH );
    state = currentState;
    count++;
    mySerial.print( "{count:" );
    mySerial.print( count );
    mySerial.print( "|millis:" );
    mySerial.print( millis() );
    mySerial.println( "}" );
  }
  delay( 10 );
}
