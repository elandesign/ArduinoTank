## Arduino Tank

A simple sketch to allow the control of a tank over wifi. The hardware is yet to be assembled (en-route from Japan & China) so this may not work at all...

![schematic](https://raw.githubusercontent.com/elandesign/Tank/master/doc/schematic.png)

## Hardware

The build will consist of

* Electronics
  * Arduino Uno
  * L9110S Dual Motor Driver Module
  * ESP8266 Wifi <-> Serial Transceiver Module
* Mechanics
  * Tamiya Universal Plate TAM70098
  * Tamiya Track and Wheel Set TAM70100
  * Tamiya Double Gearbox L/R Independ 4-speed TAM70168

## Requirements

This sketch requires the following software and libraries:

* Arduino (tested with v1.6.1 OSX)
* QueueList (http://playground.arduino.cc/Code/QueueList)
* WeeESP8266 (https://github.com/itead/ITEADLIB_Arduino_WeeESP8266), modified to use `SoftwareSerial`
* Timer (https://github.com/JChristensen/Timer)
* picoterm for make monitor task (installed via homebrew)

## Getting Started

Copy the `wifi_settings.h.default` file to `wifi_settings.h`, add your connection parameters, build, and hope for the best.
