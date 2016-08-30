cmake_minimum_required(VERSION 3.5)
project(aes_cracker)

set(CMAKE_CXX_COMPILER "/usr/gcc-4.9.3/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(SOURCE_FILES main.cpp Decoder.cpp Decoder.h Dictionary.cpp Dictionary.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} -lcryptopp)