#ifndef MYTYPES_H
#define MYTYPES_H

struct command_t {
  uint8_t header; // 0x00
  uint8_t code;
  uint8_t duration; // 10ths of a second
  uint8_t speed;
  uint8_t checksum;
};

#endif
