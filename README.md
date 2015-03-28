## Arduino Tank

A simple sketch to allow the control of a tank over wifi. The hardware is yet to be assembled (en-route from Japan & China) so this may not work at all...

## Hardware

The build will consist of

* Electronics
  * Arduino Uno
  * Motor Shield
  * ESP8266 Wifi <-> Serial Transceiver Module
  * OV7670 Camera Module
* Mechanics
  * Tamiya Universal Plate TAM70098
  * Tamiya Track and Wheel Set TAM70100
  * Tamiya Double Gearbox L/R Independ 4-speed TAM70168

## Requirements

This sketch requires the following software and libraries to be installed:

* Arduino (tested with v1.6.1 OSX)
* QueueList (http://playground.arduino.cc/Code/QueueList)
* WeeESP8266 (https://github.com/itead/ITEADLIB_Arduino_WeeESP8266), modified to use `SoftwareSerial`
* picoterm for make monitor task (installed via homebrew)

## Getting Started

Copy the `wifi.h.default` file to `wifi.h`, add your connection parameters, build, and hope for the best.
