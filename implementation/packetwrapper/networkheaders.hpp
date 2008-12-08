#ifndef NETWORK_HEADERS_HPP
#define NETWORK_HEADERS_HPP

namespace packetwrapper {

struct EthernetHeader {
	static const int MAC_ADDRESS_LENGTH = 6;
	static const unsigned short ETHER_TYPE = 0x0800;

	unsigned char dstAddress[MAC_ADDRESS_LENGTH];
	unsigned char srcAddress[MAC_ADDRESS_LENGTH];
	unsigned short type;
};

struct IPHeader{
	static const unsigned char VERSION = 4;
	static const unsigned char TYPE = 0;
	static const unsigned short IDENTIFICATION = 0;
	static const unsigned short FLAGS_FO = 0;
	static const unsigned char TTL = 0xFF;
	static const unsigned char PROTOCOL_TCP = 0x06;

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
};

struct TCPHeader {
	static const unsigned char FLAG_ACK = (1 << 4);
	static const unsigned char FLAG_RST = (1 << 2);
	static const unsigned char FLAG_SYN = (1 << 1);
	static const unsigned short WINDOW = 0x4444;

	unsigned short srcPort;
	unsigned short dstPort;
	unsigned int seq;
	unsigned int ack;
	unsigned char doff;
	unsigned char flags;
	unsigned short window;
	unsigned short checksum;
	unsigned short urgent;
};

const unsigned int FULL_HEADER_SIZE = sizeof(EthernetHeader) + sizeof(IPHeader) + sizeof(TCPHeader);

} /* namespace packetwrapper */

#endif /* NETWORK_HEADERS_HPP */
