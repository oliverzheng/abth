CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_EXECUTABLE (packetlogger main.cpp)
TARGET_LINK_LIBRARIES (packetlogger packetwrapper)
