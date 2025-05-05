# AWI Quiz Show

This firmware provides an example of how to use the UE101A (input) and UE102A (output) expansion boards as well as the UI100B AC control.

| Boards Used | UI100B  | UE101A | UI102A |
| ----------- | ------- | ------ | ------ |
|             | Control |  Input | Output |

## Overview

This firmware utilizes the UI100B (control board) as a quiz show style processor. It reads up to six (6) player button inputs using the UE101A (input expansion) and indicates which player pressed first using the UE102A (output expansion) driving LED indicators. The UI100B also controls an AC buzzer when the player hits the button. An app has been created to connect with the controller via Bluetooth to get player feedback and restart the player input scanning.
More information on the ArtistryWare line of IoT boards can be found at https://artistryware.com/iot/.

## How to use the firmware

### Firmware structure

All functions are contained in separate header and source files. The main file starts the tasks needed for functioning and then closes.

| Function  | Header     | Source     | Description                             |
| --------- | ---------- | ---------- | --------------------------------------- |
| GPIO      | qs_gpio.h  | qs_gpio.c  | GPIO configuration                      |
| I2C       | qs_i2c.h   | qs_i2c.c   | I2C initialization and processing       |
| Bluetooth | qs_blue.h  | qs_blue.c  | Bluetooth SPP connection and processing |
| Logic     | qs_logic.h | qs_logic.c | FreeRTOS tasks and function processing  |

### Hardware Required

To run this example, you should have an Artistryware UI100B main control board connected to a UE101A and UE102A expansion boards. Please feel free to contact us if you would like to purchase these boards.

#### I2C Assignments

**Note:** The following I2C pin assignments are hard wired on the UI100B. These cannot be changed. External 4.7kΩ pullups are installed so there is no need to use internal pullups.

| Board  | SDA  | SCL  |
| ------ | ---- | ---- |
| UI100B |  21  |  22  |

| Board  | Address |
| ------ | ------- |
| UE101A | 0x0024  |
| UE102A | 0x0025  |

#### UE101A Inputs

**Note** All buttons need to be momentary normally open buttons. A separate ground needs to be run to the UE101A two position connector.

Player button inputs are wired to the UE101A 12 position screw terminal connector as follows:

| Player | 1 | 2 | 3 | 4 | 5 | 6 | Restart |
| ------ | - | - | - | - | - | - | ------- |
| Input  | 1 | 2 | 3 | 4 | 5 | 6 |    7    |

#### UE102A Outputs

**Note** All indicators should be LED and less than 200mA each. A separate ground needs to be run to the UE102A two position connector.

Indicators include button LEDs and, for the front indicators, we used LED truck running lights. These are connected to the UE102A 12 position screw terminal connector as follows:

| Button | 1 | 2 | 3 | 4 | 5 | 6 |      
| ------ | - | - | - | - | - | - |      
| Output | 1 | 2 | 3 | 4 | 5 | 6 |      

| Indicator | 1 | 2 | 3 | 4  | 5  | 6  |
| --------- | - | - | - | -- | -- | -- |
| Output    | 7 | 8 | 9 | 10 | 11 | 12 |

### AWI Quiz Show App

An Android app has been created using the MIT App Inventor website and can be used to get player input feedback and restart the scanning. In order to use the app, it is first necessary to pair with the UI100B in 'Settings' using the name 'AWI_QUIZ_SHOW'. Once paired, the app will automatically connect with the board.

### Build and Flash

This firmware was written and compiled using VSCode with esp-idf extension version 5.4.1.

Enter `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

## Troubleshooting

(For any technical queries, please open an [issue](https://github.com/artistryware/Firmware-Public/issues) on GitHub. We will get back to you as soon as possible.)
