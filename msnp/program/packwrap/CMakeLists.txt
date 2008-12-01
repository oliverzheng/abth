CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_LIBRARY (packwrap packetobserver.cpp)
TARGET_LINK_LIBRARIES (packwrap libboost_system-gcc34-mt-1_37 libboost_thread-gcc34-mt-1_37)
