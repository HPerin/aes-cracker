cmake_minimum_required(VERSION 3.5)
project(aes_cracker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -march=native -std=c++14 -fopenmp")

set(SOURCE_FILES
        src/main.cpp
        src/Decoder.cpp
        src/Decoder.h
        )
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} -lcryptopp -lpthread -fopenmp)