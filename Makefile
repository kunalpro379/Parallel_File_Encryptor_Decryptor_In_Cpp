# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Include directories
INCLUDES = -I./processes -I./EncryptDecrypt -I./file_handling -I./FileCompression -I./MultiThreading

# Source files
SOURCES = main.cpp \
          processes/ProcessManagement.cpp \
          EncryptDecrypt/cryption.cpp \
          file_handling/IO.cpp \
          file_handling/ReadEnv.cpp \
          FileCompression/CompresseFile.cpp \
          MultiThreading/ThreadPool.cpp

# Output executable name
EXECUTABLE = ParallelFileEncryptor

# Libraries
LIBS = -lz -lssl -lcrypto -lstdc++fs

# Build directory
BUILD_DIR = build

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile the source files
$(BUILD_DIR)/$(EXECUTABLE): $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$(EXECUTABLE) $(SOURCES) $(LIBS)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: clean
