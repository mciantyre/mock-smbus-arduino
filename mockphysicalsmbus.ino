#include <Wire.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"

//
// B for "battery"
//
#define MY_ADDRESS        (   0x0B )
#define BUFFER_MAX_LEN    (     64 )
#define SERIAL_BAUD_RATE  ( 115200 )

// typedef doesn't seem to work here...?
// Not attempting to figure it out. #define instead
#define batt_t int16_t
#define addr_t uint8_t

typedef struct
{
  const char* key;
  const addr_t addr;
  batt_t val;
} smbus_lookup;


/**
 * Add keys, addresses, and default values here
 */
static smbus_lookup lookups[] = {
/*     Serial key   |   SMBUS command addr    |  Initial value  |   Units  */
/* -----------------|-------------------------|-----------------|--------- */
  { "VOLTAGE"            , VOLTAGE               , 11000 },       // mV
  { "CURRENT"            , CURRENT               , 2500 },        // mA
  { "MAX_ERROR"          , MAX_ERROR             , 1 },           // percent
  { "TEMPERATURE"        , TEMPERATURE           , 2900 },        // deciKelvin
  { "MNF_DATE"           , MANUFACTURER_DATE     , 12345 },       // See SMBUS documentation for bytes to date value
  { "FULL_CHARGE_CAP"    , FULL_CHARGE_CAPACITY  , 3425 },        // mAh
  { "REMAINING_CAP"      , REMAINING_CAPACITY    , 2134 }         // mAh
};
#define NUM_LOOKUPS ( sizeof( lookups ) )

/**
 * Get the value associated with key
 */
batt_t getvalkey( const char* inkey )
{
  size_t i;
  for ( i = 0; i < NUM_LOOKUPS; ++i )
  {
    if ( 0 == strcmp( lookups[i].key, inkey ) )
    {
      return lookups[i].val;
    }
  }

  return ( -1 );
}

/**
 * Get the value associated with addr
 */
batt_t getvaladdr( const addr_t addr )
{
  size_t i;
  for ( i = 0; i < NUM_LOOKUPS; ++i )
  {
    if ( addr == lookups[i].addr )
    {
      return lookups[i].val;
    }
  }

  return ( -1 );
}

/**
 * Set the value of the state identified by key
 */
int setval( const char* inkey, const batt_t val )
{
  size_t i;
  for ( i = 0; i < NUM_LOOKUPS; ++i )
  {
    if ( 0 == strcmp( lookups[i].key, inkey ) )
    {
      lookups[i].val = val;
      return 1;
    }
  }

  return 0;
}

//
// The next value to send
// when requested
//
static volatile batt_t next_send = -1;

//
// I2C callbacks
//

/**
 * Callback when bytes are received
 * Sets the next written value
 */
void receivehandler( int nbytes )
{
  char c = Wire.read();
  batt_t val = -1;
  if ( -1 != ( val = getvaladdr( c ) ) )
  {
    next_send = val;
  }
}

/**
 * Write data back to master LSB
 */
void requesthandler( void )
{
  byte tx[2] = { ( next_send >> 8 ) & 0xFF
               , ( next_send & 0xFF ) };
  Wire.write( tx, 2);
}

//
// Serial methods
//
void processSerial( char* buffer, size_t len )
{
  char * key, * value, * i;
  key = strtok_r( buffer, "=", &i );
  value = strtok_r( NULL, "=", &i );

  if ( NULL == value )
  {
    Serial.println("ERROR: key=value pair not provided");
    return;
  }

  batt_t v = atoi( value );
  if ( 0 == strcmp( value, "0" ) )
  {
    v = 0;
  }
  else if ( 0 == v )
  {
   Serial.println("ERROR: NaN");
   return; 
  }
  
  if ( 1 == setval( key, v ) )
  {
    char okmsg[BUFFER_MAX_LEN] = {0};
    snprintf( okmsg, BUFFER_MAX_LEN, "set %s=%s", key, value );
    Serial.println( okmsg );
  }
  else
  {
    Serial.println("ERROR: setting key=value failed (key not valid)");
  }
}

//
// Arduino defaults
//

void setup() {

  // init serial
  Serial.begin( SERIAL_BAUD_RATE );
  Serial.println( "Starting up..." );

    // init I2C
  Wire.begin( MY_ADDRESS );
  Wire.onReceive( receivehandler );
  Wire.onRequest( requesthandler );

}

void loop() {

  static char buffer[BUFFER_MAX_LEN] = { 0 };
  size_t c = 0;
  if ( Serial.available() > 0 )
  {
    c = Serial.readBytes( buffer, BUFFER_MAX_LEN );
    processSerial( buffer, c );
    memset( buffer, 0, sizeof( buffer ) );
    c = 0;
  }

}
