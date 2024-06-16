# Makefile for building the ConfigManager project

.PHONY: all clean

# Default target
all: build/test_configuration

# Build directory
BUILD_DIR := build

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run CMake and build the project
build/test_configuration: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && $(MAKE)
	cp $(BUILD_DIR)/test_configuration .

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	rm -f test_configuration
