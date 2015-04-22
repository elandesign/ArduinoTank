#include <SoftwareSerial.h>
#include <QueueList.h>
#include <ESP8266.h>
#include <Timer.h>
#include <Motor.h>

#include "types.h"
#include "wifi_settings.h"

// Pins
#define WIFI_RX 3
#define WIFI_TX 2
#define L_MOTOR_SPEED 4 // PWM
#define L_MOTOR_DIRECTION 5
#define R_MOTOR_SPEED 6 // PWM
#define R_MOTOR_DIRECTION 7

// Communication
#define WIFI_BAUD 9600
#define SERVER_PORT 333
#define SERVER_TIMEOUT 5

// Commands
#define CMD_FORWARD 1
#define CMD_REVERSE 2
#define CMD_TURN_R 3
#define CMD_TURN_L 4
#define CMD_STOP 5
#define CMD_RUN 6
#define CMD_CLEAR 7
#define CMD_LIST 8

#define FORWARD HIGH
#define REVERSE LOW

void executeCommand(command_t);

SoftwareSerial wifiSerial(WIFI_RX, WIFI_TX);
ESP8266 wifi(wifiSerial, WIFI_BAUD);
QueueList <command_t>queue;
Motor leftMotor(L_MOTOR_SPEED, L_MOTOR_DIRECTION);
Motor rightMotor(R_MOTOR_SPEED, R_MOTOR_DIRECTION);

bool running = true;
bool runningCommand = false;
command_t currentCommand;
Timer commandTimer;
String buffer = "";

void setupWifi()
{
  Serial.print("Connecting to WIFI... ");
  if(wifi.setOprToStation() && wifi.joinAP(WIFI_SSID, WIFI_PASSWORD))
    Serial.println("Connected");
  else
  {
    Serial.println("Failed");
    return;
  }

  Serial.print("Initialising server on " + wifi.getLocalIP() + ":" + SERVER_PORT + "... ");
  if(wifi.enableMUX() && wifi.startTCPServer(SERVER_PORT) && wifi.setTCPServerTimeout(SERVER_TIMEOUT))
    Serial.println("Success");
  else
    Serial.println("Failed");
}

void executeNextCommand()
{
  currentCommand = queue.pop();
  executeCommand(currentCommand);
}

void finishExecutingCommand()
{
  runningCommand = false;
  if(queue.isEmpty())
  {
    leftMotor.stop();
    rightMotor.stop();
  }
  else
    executeNextCommand();
}

void executeCommand(command_t command)
{
  runningCommand = true;
  switch(command.code)
  {
    case CMD_FORWARD:
      Serial.print("Forward for ");
      leftMotor.run(FORWARD, command.speed);
      rightMotor.run(FORWARD, command.speed);
      break;
    case CMD_REVERSE:
      Serial.print("Reverse for ");
      leftMotor.run(REVERSE, command.speed);
      rightMotor.run(REVERSE, command.speed);
      break;
    case CMD_TURN_L:
      Serial.print("Left for ");
      leftMotor.run(REVERSE, command.speed);
      rightMotor.run(FORWARD, command.speed);
      break;
    case CMD_TURN_R:
      Serial.print("Right for ");
      leftMotor.run(FORWARD, command.speed);
      rightMotor.run(REVERSE, command.speed);
      break;
    case CMD_STOP:
      Serial.print("Stop for ");
      leftMotor.stop();
      rightMotor.stop();
      break;
  }

  Serial.println(command.duration + " seconds at " + command.speed);
  commandTimer.after(command.duration * 100, finishExecutingCommand);
}

void parseCommand() {
  if(buffer.length() < 1)
    return;

  uint8_t code, duration, speed;
  code = buffer.charAt(0);
  duration = buffer.length() > 1 ? buffer.charAt(1) : 1;
  speed = buffer.length() > 2 ? buffer.charAt(2) : 255;

  command_t newCommand =
  {
    (uint8_t)constrain(code, CMD_FORWARD, CMD_STOP),
    (uint8_t)constrain(duration, 0, 10),
    (uint8_t)constrain(speed, 0, 255)
  };

  queue.push(newCommand);

  buffer = "";
}

void readCommand()
{
  command_t command;
  uint8_t buffer[sizeof(command)] = {0};
  uint8_t mux_id;
  uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);

  if(len > 0)
  {
    Serial.print("Received ");
    Serial.print(len);
    Serial.println(" bytes");
    Serial.print("  ");
    for(uint8_t i=0; i<sizeof(buffer); i++)
    {
      Serial.print(buffer[i]);
      Serial.print("  ");
    }
    Serial.println();
  }

  if(len == sizeof(command) && buffer[0] == 0)
  {
    memcpy(&command, buffer, sizeof(command));

    Serial.print("Command: ");
    Serial.print(command.code);
    Serial.print(",");
    Serial.print(command.duration);
    Serial.print(",");
    Serial.println(command.speed);

    queue.push(command);
    Serial.print("Queue: ");
    Serial.println(queue.count());
  }
}

void setup()
{
  Serial.begin(57600);
  setupWifi();
}

void loop()
{
  commandTimer.update();
  readCommand();
  if(running && !queue.isEmpty() && !runningCommand)
    executeNextCommand();
}
