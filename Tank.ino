#include <SoftwareSerial.h>
#include <QueueList.h>
#include <ESP8266.h>

#include "types.h"
#include "wifi.h"
#include "motor.h"

// Pins
#define WIFI_RX 3
#define WIFI_TX 2
#define L_MOTOR_SPEED 9 // PWM
#define L_MOTOR_DIRECTION 8
#define R_MOTOR_SPEED 11 // PWM
#define R_MOTOR_DIRECTION 10

// Communication
#define WIFI_BAUD 57600
#define SERVER_PORT 333
#define SERVER_TIMEOUT 5

// Debugging
#define debug Serial

// Commands
#define CMD_FORWARD 1
#define CMD_REVERSE 2
#define CMD_TURN_R 3
#define CMD_TURN_L 4
#define CMD_RUN 5
#define CMD_STOP 6
#define CMD_CLEAR 7
#define CMD_LIST 8

#define FORWARD HIGH
#define REVERSE LOW

SoftwareSerial wifiSerial(WIFI_RX, WIFI_TX);
ESP8266 wifi(wifiSerial, WIFI_BAUD);
QueueList <command_t>queue;
Motor leftMotor(L_MOTOR_SPEED, L_MOTOR_DIRECTION);
Motor rightMotor(R_MOTOR_SPEED, R_MOTOR_DIRECTION);

bool running = false;
bool runningCommand = false;
command_t command;

void setupWifi() {
  debug.print("Connecting to WIFI... ");
  if(wifi.setOprToStation() && wifi.joinAP(WIFI_SSID, WIFI_PASSWORD))
    debug.println("Connected");
  else {
    debug.println("Failed");
    return;
  }

  debug.print("Initialising server on " + wifi.getLocalIP() + ":" + SERVER_PORT + "... ");
  if(wifi.enableMUX() && wifi.startTCPServer(SERVER_PORT) && wifi.setTCPServerTimeout(SERVER_TIMEOUT))
    debug.println("Success");
  else
    debug.println("Failed");
}

void executeCommand(command_t* command) {
  runningCommand = true;
  switch(command->code) {
    case CMD_FORWARD:
      leftMotor.run(FORWARD, command->speed);
      rightMotor.run(FORWARD, command->speed);
      break;
    case CMD_REVERSE:
      leftMotor.run(REVERSE, command->speed);
      rightMotor.run(REVERSE, command->speed);
      break;
    case CMD_TURN_L:
      leftMotor.run(REVERSE, command->speed);
      rightMotor.run(FORWARD, command->speed);
      break;
    case CMD_TURN_R:
      leftMotor.run(FORWARD, command->speed);
      rightMotor.run(REVERSE, command->speed);
      break;
  }
}

void finishExecutingCommand() {
  runningCommand = false;
}

void setup()
{
  debug.begin(57600);
  setupWifi();
}

void loop()
{
  if(running && !queue.isEmpty() && !runningCommand)
    command = queue.pop();
    executeCommand(&command);
}
