
A wrapping class to operate a MC33926 motor driver.

[Pololu Single Driver](https://www.pololu.com/product/1212)<br>
[Pololu Dual Driver](https://www.pololu.com/product/1213)<br>
[MC33926 Datasheet](https://pololu.com/file/0J233/MC33926.pdf)

# How to Use

## Setup and Set Speed

```
    #include <MC33926>

    #define IN1_PIN 6
    #define IN2_PIN 9

    MC33926 motorController(IN1_PIN, IN2_PIN);

    motorController.setSpeed(100);
```

## Current Sense 

Set the pin on which to read current sense values.

`void setCurrentSensePin(int fbCurrentSensePin, float referenceVoltage)`

Returns a float value representing the current in milliamps.

`float getCurrent()`

Note, this feature is currently untested.

## Status Flag

You can actively read the status flag sense pin at-will or register an interrupt handler to execute upon a status change event. Either must be invoked at setup time.

`void registerStatusChangeHandler(void (*statusServiceRoutine)(void), int statusFlagPin)`

`void setStatusFlagPin(int statusFlagPin)`

Returns true if the mc33926 has set the status flag.

`boolean getStatus()`

# Expected Logic Connections

3.3v or 5v is allowed. All values below are based on 5v

## Input Pins (to MC33926)
|Pin  | Level |
|:---:|:-----:|
|VDD  |5v     |
|EN   |5v     |
|D1   |0v     |
|D2   |5v     |
|INV  |0v     |
|SLEW |0v for low freq pwm, 5v for high freq pwm|
|IN1 (fwd)|PWM    |
|IN2 (fwd)|0v     |
|IN1 (reverse)|0v     |
|IN2 (reverse)|PWM    |

For the Dual MC33926 VDD, EN, INV, and SLEW cover both controllers. D1, D2 and IN1, IN2 are duplicated.

## Output Pins (from MC33926)
|Pin  | Level |
|:---:|:-----:|
|FB   |0v w/ pull-down resistor (analog)|
|SF   |5v w/ pull-up resistor (digital) |

# Arduino Pin Information

Uno pwm pins:

3, 5, 6, 9, 10, 11, 490 Hz (pins 5 and 6: 980 Hz)

Due pwm pins:

2-13, 1000 Hz

use analogWriteResolution to set higher resolution. i.e. "12" for 12-bit -> 4096bit resolution

pins DAC0 and DAC1 are "true" analog

# MC33926 Breakout

The dual MC33926 board has a large number of configuration pins. Many of these are easily set using a jumper with the onboard VDD or GND connections. But other optional configurations can require a large number of VDD or GND connections.

This [dual breakout board](schematics) provides eight VDD and eight GND connections for various settings. The board also provides connections for a four-wire ribbon cable to the dual IN1 and IN2 connections.

[Breakout Board](docs/mc33926_breakout1.jpg)

[Example implementation](docs/mc33926_breakout2.jpg)