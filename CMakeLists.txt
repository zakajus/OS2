cmake_minimum_required(VERSION 4.0)
project(OS2)

set(CMAKE_CXX_STANDARD 20)

add_executable(OS2 main.cpp
        channelDevice.cpp
        hardDisk.cpp
        keyboard.cpp
        monitor.cpp
        realMachine.cpp
        virtualMachine.cpp
        )

