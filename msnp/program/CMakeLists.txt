CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (ABTH)

INCLUDE_DIRECTORIES (${ABTH_SOURCE_DIR}) 

LINK_DIRECTORIES (${ABTH_BINARY_DIR}/lib) 
SET (EXECUTABLE_OUTPUT_PATH ${ABTH_BINARY_DIR}/bin)

SET (CMAKE_BUILD_TYPE Debug)

#############################
#        PCAP SETUP         *
#############################

IF (WIN32)
	SET (PCAP_DIR winpcap)
ELSE (WIN32)
	SET (PCAP_DIR libpcap)
ENDIF (WIN32)

INCLUDE_DIRECTORIES (${ABTH_SOURCE_DIR}/pcap/${PCAP_DIR}/include)

#############################
#      PROJECTS SETUP       *
#############################

ADD_SUBDIRECTORY (packetwrapper)
ADD_SUBDIRECTORY (logger)
