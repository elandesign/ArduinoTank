#include <SoftwareSerial.h>
#include <QueueList.h>
#include <ESP8266.h>

#include "types.h"
#include "wifi.h"

// Pins
#define WIFI_RX 3
#define WIFI_TX 2

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

SoftwareSerial wifiSerial(WIFI_RX, WIFI_TX);
QueueList <command_t>queue;
ESP8266 wifi(wifiSerial, WIFI_BAUD);
bool run = false;
bool executingCommand = false;
command_t currentCommand;

void setup()
{
  debug.begin(57600);

  setupWifi();
}

void setupWifi() {
  debug.print("Connecting to WIFI... ");
  if(wifi.setOprToStation() && wifi.joinAP(WIFI_SSID, WIFI_PASSWORD))
    debug.println("Connected");
  else
    debug.println("Failed");

  debug.print("Initialising server on " + wifi.getLocalIP() + ":" + SERVER_PORT + "... ");
  if(wifi.enableMUX() && wifi.startTCPServer(SERVER_PORT) && wifi.setTCPServerTimeout(SERVER_TIMEOUT))
    debug.println("Success");
  else
    debug.println("Failed");
}

void loop()
{
  if(run && !queue.isEmpty() && !executingCommand)
    currentCommand = queue.pop();
    executeCommand(&currentCommand);
}

void executeCommand(command_t* command) {
  executingCommand = true;
  switch(command->code) {
    case CMD_FORWARD:
      break;
    case CMD_REVERSE:
      break;
    case CMD_TURN_L:
      break;
    case CMD_TURN_R:
      break;
  }
}

void finishExecutingCommand() {
  executingCommand = false;
}
