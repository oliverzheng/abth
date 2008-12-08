#ifndef NETWORK_HEADERS_HPP
#define NETWORK_HEADERS_HPP

#define MAC_ADDRESS_LENGTH	6

namespace packetwrapper {

typedef struct EthernetHeader {
	unsigned char dstAddress[MAC_ADDRESS_LENGTH];
	unsigned char srcAddress[MAC_ADDRESS_LENGTH];
	unsigned short type;
} EthernetHeader;

typedef struct IPHeader{
	unsigned char version_hdrlength;
	unsigned char type;
	unsigned short length;
	unsigned short identification;
	unsigned short flags_fo;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short checksum;
	unsigned int srcAddress;
	unsigned int dstAddress;
} IPHeader;

typedef struct TCPHeader {
	unsigned short srcPort;
	unsigned short dstPort;
	unsigned int seq;
	unsigned int ack;
	unsigned char doff;
	unsigned char flags;
	unsigned char window;
	unsigned short checksum;
	unsigned short urgent;
} TCPHeader;

} /* namespace packetwrapper */

#endif /* NETWORK_HEADERS_HPP */
