CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_EXECUTABLE (msnpabth
	msnppacket.cpp
	msnpabth.cpp
	main.cpp
)
TARGET_LINK_LIBRARIES (msnpabth packetwrapper)
