#include <SendOnlySoftwareSerial.h>

#define LED 1
#define BTN 2

SendOnlySoftwareSerial mySerial( 4 ); // TX

int state = 0;
int count = 0;

void setup() {
  mySerial.begin( 19200 );
  pinMode( BTN, INPUT_PULLUP );
  pinMode( LED, OUTPUT );
}

void loop() {
  int currentState = digitalRead( BTN );
  
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
