## mockphysicalsmbus

A sketch to have an Arduino pretend as if it's an SMBUS-compliant battery. Hook the Arduino up to hardware that accepts an SMBUS battery, and it will respond with values when queried. Values are runtime-configurable.

## Getting started

Edit (or add to) the lookup table in the sketch depending on the parameters needed by your system.

```c
static smbus_lookup lookups[] = {
/*     Serial key   |   SMBUS command addr    |  Initial value  |   Units  */
/* -----------------|-------------------------|-----------------|--------- */
  { "VOLTAGE"            , VOLTAGE               , 11000 },       // mV
  { "CURRENT"            , CURRENT               , 2500 },        // mA
  { "MAX_ERROR"          , MAX_ERROR             , 1 },           // percent
  { "TEMPERATURE"        , TEMPERATURE           , 2900 },        // deciKelvin
  { "FULL_CHARGE_CAP"    , FULL_CHARGE_CAPACITY  , 3425 },        // mAh
  { "REMAINING_CAP"      , REMAINING_CAPACITY    , 2134 }         // mAh
};
```

`Serial key` is the key you will use to change the values during runtime. The `SMBUS command addr` is a command address available in `commands.h`. See the SMBUS specification for specific command descriptions. Provide an initial value for your parameter as well.

Compile and upload it to an Arduino (tested on Arduino Uno R3). Open up the serial monitor with a baud rate of `115200`, and you should see the text "Starting up..." Additionally, to change SMBUS values, the serial monitor must be configured for "No newline endings." 

To change SMBUS values, use the syntax `[serial_key]=[new_value]`. For instance, if I wanted to change the reported full charge capacity to 6800 mAh, I would send

```
FULL_CHARGE_CAP=6800
```

using the serial monitor. Note that I used the `Serial key` with no quotes, not the `SMBUS command addr`. If all is successful, the Arduino will respond with the new value, and the new value will be sent to the connected system when queried.