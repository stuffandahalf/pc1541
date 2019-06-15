cmake_minimum_required (VERSION 2.6)
project(pc1541)
include_directories(include)
#set(SOURCES src/main.cxx src/CBM1541.cxx src/Memory.cxx src/MOS6502.cxx src/MOS6522.cxx src/Registers.cxx src/ArduinoInterface.cxx
file(GLOB SOURCES "src/*.cxx")

add_executable(pc1541 ${SOURCES})
