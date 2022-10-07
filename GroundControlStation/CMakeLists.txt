cmake_minimum_required(VERSION 3.1)
project(GCS)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

if(WIN32)
add_executable(GCS
        config.h
        datalink.cpp
        datalink.h
        ether.h
        ether.cpp
        main.cpp
        serial.cpp
        serial.h)
else()
add_executable(GCS
        config.h
        datalink.cpp
        datalink.h
        ether.h
        ether2.cpp
        main.cpp
        serial.cpp
        serial.h)
target_link_libraries(GCS stdc++)
endif()