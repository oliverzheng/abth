CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

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
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
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

SET (PCAP_LIB_DIRECTORY ${ABTH_SOURCE_DIR}/pcap/${PCAP_DIR}/lib)

IF (UNIX)
	SET (PCAP_LIB_DEPENDENCY wpcap)
ELSE (UNIX)
	SET (PCAP_LIB_DEPENDENCY ${PCAP_LIB_DIRECTORY}/wpcap.lib)
ENDIF (UNIX)

LINK_DIRECTORIES (${PCAP_LIB_DIRECTORY})

#############################
#      BUILD LIBRARY        #
#############################

ADD_LIBRARY (packetwrapper STATIC
	interface.cpp
	interfaceaddress.cpp
	interfaceapp.cpp
	ipaddress.cpp
	packetobserver.cpp
	tcppacket.cpp
)
TARGET_LINK_LIBRARIES (packetwrapper ${Boost_LIBRARIES} ${PCAP_LIB_DEPENDENCY})

IF (NOT UNIX)
	TARGET_LINK_LIBRARIES(packetwrapper Ws2_32.lib)
ENDIF (NOT UNIX)
