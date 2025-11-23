cmake_minimum_required(VERSION 4.0)
project(OS2)

set(CMAKE_CXX_STANDARD 20)

add_executable(OS2 src/main.cpp
        src/components/channelDevice.cpp
        src/components/keyboard.cpp
        src/components/monitor.cpp
        src/components/realMachine.cpp
        src/processes/virtualMachine.cpp
        )

