#include <Motor.h>
#include <SPI.h>
#include <Adafruit_BLE_UART.h>
#include <Timer.h>

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9

// Pins
#define L_MOTOR_SPEED 5 // PWM
#define L_MOTOR_DIRECTION 4
#define R_MOTOR_SPEED 6 // PWM
#define R_MOTOR_DIRECTION 7

#define RUNTIME 1000

#define FORWARD 1
#define REVERSE 0

// Debugging
#define debug Serial

// Commands
#define CMD_FORWARD 0x32
#define CMD_REVERSE 0x38
#define CMD_TURN_R 0x36
#define CMD_TURN_L 0x34

Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
Motor leftMotor(L_MOTOR_SPEED, L_MOTOR_DIRECTION);
Motor rightMotor(R_MOTOR_SPEED, R_MOTOR_DIRECTION);

bool running = false;
Timer commandTimer;

void finishExecutingCommand()
{
  leftMotor.stop();
  rightMotor.stop();
  running = false;
}

void executeCommand(uint8_t command)
{
  running = true;
  switch(command)
  {
    case CMD_FORWARD:
      leftMotor.run(FORWARD, 255);
      rightMotor.run(FORWARD, 255);
      break;
    case CMD_REVERSE:
      leftMotor.run(REVERSE, 255);
      rightMotor.run(REVERSE, 255);
      break;
    case CMD_TURN_L:
      leftMotor.run(REVERSE, 255);
      rightMotor.run(FORWARD, 255);
      break;
    case CMD_TURN_R:
      leftMotor.run(FORWARD, 255);
      rightMotor.run(REVERSE, 255);
      break;
  }

  commandTimer.after(RUNTIME, finishExecutingCommand);
}

void aciCallback(aci_evt_opcode_t event)
{
  switch(event)
  {
    case ACI_EVT_DEVICE_STARTED:
      break;
    case ACI_EVT_CONNECTED:
      break;
    case ACI_EVT_DISCONNECTED:
      leftMotor.stop();
      rightMotor.stop();
      break;
    default:
      break;
  }
}

void rxCallback(uint8_t *buffer, uint8_t len)
{
  if(!running)
    executeCommand(buffer[0]);
}

void setup()
{
  uart.setRXcallback(rxCallback);
  uart.setACIcallback(aciCallback);
  uart.setDeviceName("Tank"); /* 7 characters max! */
  uart.begin();
}

void loop()
{
  commandTimer.update();
  uart.pollACI();
}
