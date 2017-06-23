#include "config.h"
#include <SoftwareSerial.h>

#define RED_LED 0
#define BLUE_LED 2

SoftwareSerial mySerial( 14, 12 ); // RX, TX
String inputString = ""; // a string to hold incoming data
int len = 0;
boolean stringComplete = false;  // whether the string is complete

AdafruitIO_Feed *ifttt = io.feed( "ifttt" );
AdafruitIO_Feed *counter = io.feed( "counter" );

void setup() {
  pinMode( RED_LED, OUTPUT );
  digitalWrite( RED_LED, HIGH );
  pinMode( BLUE_LED, OUTPUT );
  digitalWrite( BLUE_LED, HIGH );

  Serial.begin( 115200 );
  Serial.print( "Connecting to Adafruit IO" );

  // connect to io.adafruit.com
  io.connect();
  
  // wait for a connection
  while ( io.status() < AIO_CONNECTED ) {
    Serial.print( "." );
    delay( 500 );
  }
  digitalWrite( BLUE_LED, LOW );

  // we are connected
  Serial.println();
  Serial.println( io.statusText() );

  ifttt->onMessage(handleIfttt);

  Serial.println( "Waiting for data..." );
  inputString.reserve( 200 ); // reserve a buffer in memory for 200 chars max
  mySerial.begin( 19200 );
}

void loop() {
  io.run();

  while ( mySerial.available() && !stringComplete ) {
    digitalWrite( RED_LED, LOW );
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
    Serial.print( "sending -> " );
    Serial.println( inputString );
    char buf[ len ];
    inputString.toCharArray( buf, len );
    Serial.println( counter->save( buf ) );
    inputString = "";
    len = 0;
    stringComplete = false;
    digitalWrite( RED_LED, HIGH );
  }
}

void checkWifi() {
  if ( io.status() < AIO_CONNECTED ) {
    Serial.print( "Connecting to Adafruit IO" );

    // connect to io.adafruit.com
    io.connect();
    while ( io.status() < AIO_CONNECTED ) {
      Serial.print( "." );
      delay( 500 );
    }
  }
  analogWrite( BLUE_LED, 127 + sin( millis() / 100. ) );
}

void handleIfttt( AdafruitIO_Data *data ) {
  Serial.print( "received <- " );

  // since we are using the same function to handle
  // messages for two feeds, we can use feedName() in
  // order to find out which feed the message came from.
  Serial.print( data->feedName() );
  Serial.print( " " );

  // print out the received count or counter-two value
  Serial.println( data->value() );
  mySerial.println( "get" );

}
