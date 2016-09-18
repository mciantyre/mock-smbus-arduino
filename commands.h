#ifndef COMMANDS_H
#define COMMANDS_H

/* SMBus commands */
#define MANUFACTURER_ACCESS                 0x00
#define REMAINING_CAPACITY_ALARM            0x01
#define REMAINING_TIME_ALARM                0x02
#define BATTERY_MODE                        0x03
#define AT_RATE                             0x04

/* Commands below are read only */
#define AT_RATE_TIME_TO_FULL                0x05
#define AT_RATE_TIME_TO_EMPTY               0x06
#define AT_RATE_OK                          0x07
#define TEMPERATURE                         0x08
#define VOLTAGE                             0x09
#define CURRENT                             0x0a
#define AVERAGE_CURRENT                     0x0b
#define MAX_ERROR                           0x0c
#define RELATIVE_STATE_OF_CHARGE            0x0d
#define ABSOLUTE_STATE_OF_CHARGE            0x0e
#define REMAINING_CAPACITY                  0x0f
#define FULL_CHARGE_CAPACITY                0x10
#define RUN_TIME_TO_EMPTY                   0x11
#define AVERAGE_TIME_TO_EMPTY               0x12
#define AVERAGE_TIME_TO_FULL                0x13
#define CHARGING_CURRENT                    0x14
#define CHARGING_VOLTAGE                    0x15
#define BATTERY_STATUS                      0x16
#define CYCLE_COUNT                         0x17
#define DESIGN_CAPACITY                     0x18
#define DESIGN_VOLTAGE                      0x19
#define SPECIFICATION_INFO                  0x1a
#define MANUFACTURER_DATE                   0x1b
#define SERIAL_NUMBER                       0x1c
#define MANUFACTURER_NAME                   0x20
#define DEVICE_NAME                         0x21
#define DEVICE_CHEMISTRY                    0x22
#define MANUFACTURER_DATA                   0x23

#endif // COMMANDS_H
