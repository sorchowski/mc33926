
SKETCH_PATH ?= src/mc33926/examples/mc33926/
SKETCH_NAME ?= mc33926.ino

TARGET = $(SKETCH_PATH)/$(SKETCH_NAME)

LIBRARY_NAME = mc33926

# Arduino CLI executable name and directory location
ARDUINO_CLI = arduino-cli

# Arduino CLI Board type
BOARD_TYPE ?= arduino\:avr\:uno

# Default port to upload to
SERIAL_PORT ?= COM4

# Build path -- used to store built binary and object files
BUILD_DIR ?= build

WIN_LIBRARY_PATH = /mnt/c/Users/steve/Documents/Arduino/libraries

.PHONY: all sketch sketch-verbose upload clean install-lib uninstall-lib zip

all: sketch

sketch:
	$(ARDUINO_CLI) compile --build-path=$(BUILD_DIR) --build-cache-path=$(BUILD_DIR) -b $(BOARD_TYPE) $(TARGET)

sketch-verbose:
	$(ARDUINO_CLI) compile -v --build-path=$(BUILD_PATH) --build-cache-path=$(BUILD_PATH) -b $(BOARD_TYPE) $(TARGET)

upload:
	$(ARDUINO_CLI) upload $(VERBOSE) -p $(SERIAL_PORT) --fqbn $(BOARD_TYPE) $(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
	@rm $(BUILD_DIR)/*.elf
	@rm $(BUILD_DIR)/*.hex

install-lib:
	$(ARDUINO_CLI) lib install --verbose --zip-path build/$(LIBRARY_NAME).zip

uninstall-lib:
	$(ARDUINO_CLI) lib uninstall $(LIBRARY_NAME)

install-winlib:
	unzip $(BUILD_DIR)/$(LIBRARY_NAME).zip -d $(WIN_LIBRARY_PATH)

uninstall-winlib:
	rm -r $(WIN_LIBRARY_PATH)/$(LIBRARY_NAME)

zip:
	mkdir -p $(BUILD_DIR)
	# ignore the output of rm on the next line
	-rm $(BUILD_DIR)/$(LIBRARY_NAME).zip
	(cd src/ && zip -FSr - $(LIBRARY_NAME)) > $(BUILD_DIR)/$(LIBRARY_NAME).zip

install-win: zip install-winlib