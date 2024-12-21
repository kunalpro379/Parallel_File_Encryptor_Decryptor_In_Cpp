cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ParallelFileEncryptor)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(processes EncryptDecrypt file_handling)

# Add the executable
add_executable(ParallelFileEncryptor 
    main.cpp 
    processes/ProcessManagement.cpp 
    EncryptDecrypt/cryption.cpp
)

# Optionally, you can add any additional libraries you need
# target_link_libraries(ParallelFileEncryptor <your-libraries>)