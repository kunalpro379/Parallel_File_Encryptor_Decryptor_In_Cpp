# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
INCLUDE_DIR = include
LIB_DIR = lib

# Source files
SOURCES = main.cpp \
           processes/ProcessManagement.cpp \
           EncryptDecrypt/cryption.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = $(BIN_DIR)/ParallelFileEncryptor

# Default target
all: $(EXECUTABLE)

# Linking
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)