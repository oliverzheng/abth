CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

#############################
#        BOOST SETUP        #
#############################

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD ON)
SET (Boost_ADDITIONAL_VERSIONS "1.37")
FIND_PACKAGE (Boost REQUIRED COMPONENTS
	regex
	system
)

IF (Boost_FOUND)
	INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
ENDIF (Boost_FOUND)

ADD_EXECUTABLE (mixer
	main.cpp
	mixerconnection.cpp
	mixerserver.cpp
)

TARGET_LINK_LIBRARIES (mixer ${Boost_LIBRARIES})
IF (WIN32)
	TARGET_LINK_LIBRARIES (mixer ws2_32 mswsock)
ENDIF (WIN32)
