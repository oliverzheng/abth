CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (ABTH)

INCLUDE (DefineFlags.cmake)

INCLUDE_DIRECTORIES (${ABTH_SOURCE_DIR}) 

LINK_DIRECTORIES (${ABTH_BINARY_DIR}/lib) 
SET (EXECUTABLE_OUTPUT_PATH ${ABTH_BINARY_DIR}/bin)

#############################
#        BOOST SETUP        #
#############################

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD ON)
SET (Boost_ADDITIONAL_VERSIONS "1.37")
FIND_PACKAGE (Boost REQUIRED COMPONENTS
	date_time
	thread
	regex
)

IF (Boost_FOUND)
	INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
ENDIF (Boost_FOUND)

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
ADD_SUBDIRECTORY (msnplogger)
ADD_SUBDIRECTORY (msnpabth)
ADD_SUBDIRECTORY (msnp)
