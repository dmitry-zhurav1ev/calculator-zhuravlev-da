# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c11 -I./src
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror -I./src
LDFLAGS = -lm
GTEST_LIBS = -lgtest -lgtest_main -lpthread

# Directories
SRC_DIR = src
TEST_DIR = tests/unit
BUILD_DIR = build

# Source files
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/calculator.c
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)

# Phony targets
.PHONY: all clean test run-int run-float run-unit-test format

# Default target
all: format app unit-tests

# Build targets
app: $(BUILD_DIR)/app
unit-tests: $(BUILD_DIR)/unit-tests

# Run targets
run-int: app
	@$(BUILD_DIR)/app

run-float: app
	@$(BUILD_DIR)/app --float

run-unit-test: unit-tests
	@$(BUILD_DIR)/unit-tests

# Formatting
format:
	@echo "Formatting code..."
	@find $(SRC_DIR) $(TEST_DIR) -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format -i -style=file

# Main application build
$(BUILD_DIR)/app: $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Unit tests build
$(BUILD_DIR)/unit-tests: $(SRC_DIR)/calculator.c $(TEST_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/calculator.c -o $(BUILD_DIR)/calculator.o
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(BUILD_DIR)/calculator.o $(GTEST_LIBS) $(LDFLAGS) -o $@

# Clean artifacts
clean:
	@rm -rf $(BUILD_DIR)