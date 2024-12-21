#!/bin/bash

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra"

# Include directories
INCLUDES="-I./processes -I./EncryptDecrypt -I./file_handling -I./FileCompression"

# Source files
SOURCES="main.cpp \
         processes/ProcessManagement.cpp \
         EncryptDecrypt/cryption.cpp \
         file_handling/IO.cpp \
         file_handling/ReadEnv.cpp"

# Output executable name
EXECUTABLE="ParallelFileEncryptor.exe"

# Libraries
LIBS="-lz -lstdc++fs"

# Create build directory if it doesn't exist
mkdir -p build

# Compile the source files
$CXX $CXXFLAGS $INCLUDES -o build/$EXECUTABLE $SOURCES $LIBS

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Executable created: build/$EXECUTABLE"
else
    echo "Compilation failed!"
    exit 1
fi
