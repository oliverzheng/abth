CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_EXECUTABLE (logger
	main.cpp
	logger.cpp
)
TARGET_LINK_LIBRARIES (logger packetwrapper)
