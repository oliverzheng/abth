CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_EXECUTABLE (msnplogger
	main.cpp
	logger.cpp
)

TARGET_LINK_LIBRARIES (msnplogger msnp packetwrapper)
