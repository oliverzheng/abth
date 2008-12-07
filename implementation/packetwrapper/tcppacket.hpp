#ifndef TCP_PACKET_HPP
#define TCP_PACKET_HPP

#include <config/config.hpp>
#include "ipaddress.hpp"

namespace packetwrapper {

class TCPPacket
{
public:

	TCPPacket();
	TCPPacket(TCPPacket & other);

	~TCPPacket();

	bool parse(const unsigned char * payload, unsigned int payloadLength);

	unsigned int srcPort;
	IPAddress srcIP;

	unsigned int dstPort;
	IPAddress dstIP;

	unsigned int seq;
	unsigned int ack;

	bool ackFlag;
	bool rstFlag;
	bool synFlag;

	unsigned int length;

	/*
	 * Data of the packet may not be completely captured.
	 * This represents the length of the captured data.
	 */
	unsigned int dataLength;

	/*
	 * TCPPacket owns this data.
	 */
	const unsigned char * data;

}; /* class TCPPacket */

} /* namespace packetwrapper */

#endif
