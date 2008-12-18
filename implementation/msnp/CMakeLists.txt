CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_LIBRARY (msnp STATIC
    msnpexceptions.hpp
    msnppacket.hpp
	msnppacket.cpp
)

TARGET_LINK_LIBRARIES (msnp)
