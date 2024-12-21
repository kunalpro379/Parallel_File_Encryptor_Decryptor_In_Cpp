#!/bin/bash

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra"

# Set the path to your vcpkg installation
VCPKG_ROOT="C:/vcpkg"  # Change this to your actual vcpkg path

# Include directories
INCLUDES="-I./processes -I./EncryptDecrypt -I./file_handling -I./FileCompression -I./OpenSSL -I$VCPKG_ROOT/installed/x64-windows/include"

# Source files
SOURCES="main.cpp \
         processes/ProcessManagement.cpp \
         EncryptDecrypt/cryption.cpp \
         file_handling/IO.cpp \
         file_handling/ReadEnv.cpp \
         FileCompression/CompresseFile.cpp \
         MultiThreading/ThreadPool.cpp \
         OpenSSL/OpenSSLCryption.cpp"

# Output executable name
EXECUTABLE="ParallelFileEncryptor.exe"

# Libraries
LIBS="-L$VCPKG_ROOT/installed/x64-windows/lib -lz -lssl -lcrypto -lstdc++fs"

# Compile the source files in the current directory
echo "Compiling..."
$CXX $CXXFLAGS $INCLUDES -o $EXECUTABLE $SOURCES $LIBS

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Executable created: ./$EXECUTABLE"
else
    echo "Compilation failed!"
    exit 1
fi