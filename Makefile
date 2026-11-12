SKETCH_PATH ?= src/mc33926/examples/mc33926
SKETCH_NAME ?= mc33926.ino

TARGET = $(SKETCH_PATH)/$(SKETCH_NAME)

LIBRARY_NAME = mc33926

# Arduino CLI Board type
BOARD_TYPE ?= arduino\:avr\:uno

# Build path -- used to store built binary and object files
BUILD_DIR ?= build

# Usage: $(call check_defined, VAR1 VAR2 ..., optional hint)
# Fails the make run if any listed variable is undefined or empty.
check_defined = $(strip $(foreach v,$1, \
    $(if $(value $v),, \
        $(error $v is not defined$(if $2, ($(strip $2)))))))

.PHONY: all sketch sketch-verbose clean install-lib uninstall-lib \
        install-winlib uninstall-winlib check-winlib zip install install-win

# Run targets one at a time so clean -> zip -> install keep their order under make -j
.NOTPARALLEL:

all: sketch

sketch:
	@:$(call check_defined, BUILD_DIR BOARD_TYPE TARGET)
	arduino-cli compile --build-path=$(BUILD_DIR) -b $(BOARD_TYPE) $(TARGET)

sketch-verbose:
	@:$(call check_defined, BUILD_DIR BOARD_TYPE TARGET)
	arduino-cli compile -v --build-path=$(BUILD_DIR) -b $(BOARD_TYPE) $(TARGET)

clean:
	@:$(call check_defined, BUILD_DIR)
	@rm -rf $(BUILD_DIR)

install-lib:
	@:$(call check_defined, BUILD_DIR LIBRARY_NAME)
	arduino-cli lib install --verbose --zip-path $(BUILD_DIR)/$(LIBRARY_NAME).zip

uninstall-lib:
	@:$(call check_defined, LIBRARY_NAME)
	arduino-cli lib uninstall $(LIBRARY_NAME)

# Guard target: list first in prerequisites so it fails before other steps run
check-winlib:
	@:$(call check_defined, ARDUINO_WIN_LIBRARY_PATH, path to your Windows Arduino libraries folder)

install-winlib: check-winlib
	@:$(call check_defined, BUILD_DIR LIBRARY_NAME)
	rm -rf "$(ARDUINO_WIN_LIBRARY_PATH)/$(LIBRARY_NAME)"
	unzip $(BUILD_DIR)/$(LIBRARY_NAME).zip -d "$(ARDUINO_WIN_LIBRARY_PATH)"

uninstall-winlib: check-winlib
	@:$(call check_defined, LIBRARY_NAME)
	rm -r "$(ARDUINO_WIN_LIBRARY_PATH)/$(LIBRARY_NAME)"

zip:
	@:$(call check_defined, BUILD_DIR LIBRARY_NAME)
	mkdir -p $(BUILD_DIR)
	rm -f $(BUILD_DIR)/$(LIBRARY_NAME).zip
	(cd src/ && zip -r - $(LIBRARY_NAME)) > $(BUILD_DIR)/$(LIBRARY_NAME).zip

install: clean zip install-lib

install-win: check-winlib clean zip install-winlib
