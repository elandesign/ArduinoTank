## Arduino Tank

A simple sketch to allow the control of a tank over Bluetooth LE.

![schematic](https://raw.githubusercontent.com/elandesign/Tank/bluetooth-control/doc/schematic.png)

## Hardware

The build consists of

* Electronics
  * Arduino Uno
  * L9110S Dual Motor Driver Module
  * Adafruit nrf8001 BLE Breakout Board
* Mechanics
  * Tamiya Universal Plate TAM70098
  * Tamiya Track and Wheel Set TAM70100
  * Tamiya Double Gearbox L/R Independ 4-speed TAM70168

## Requirements

This sketch requires the following software and libraries:

* Arduino (tested with v1.6.1 OSX)
* Adafruit nrf8001 (https://github.com/adafruit/Adafruit_nRF8001)
* Timer (https://github.com/JChristensen/Timer)
* picoterm for make monitor task (installed via homebrew)

# Notes

Commands can be sent to the tank using [Blue Remote Free](https://itunes.apple.com/us/app/blue-remote-free/id851124912?mt=8)

2 = Forwards
8 = Backwards
4 = Left
6 = Right

(because I couldn't be bothered with multi-byte commands at this stage)

Each command will run for 1 second, then the tank will stop.

The 47µF capacitor was added to stop the Arduino restarting every time the motor started


![prototype](https://raw.githubusercontent.com/elandesign/Tank/bluetooth-control/doc/tank.jpg)
