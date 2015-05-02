BOARD_TAG        = uno
USER_LIB_PATH    = $(realpath libraries)
ARDUINO_LIBS     = SoftwareSerial ESP8266 Motor QueueList Timer
MONITOR_CMD      = picocom

ifndef ARDMK_DIR
	ARDMK_DIR = $(realpath libraries/Arduino-Makefile)
endif

include $(ARDMK_DIR)/Arduino.mk
