#ifndef TCP_PACKET_HPP
#define TCP_PACKET_HPP

/* Project Includes */
#include <config/config.hpp>
#include "ipaddress.hpp"
#include "packetobserver.hpp"

namespace packetwrapper {

/* Resolve circular dependency */
class PacketObserver;

/* Abstraction of a TCP packet.
 * Users may inherit from this class for application-specific packets.
 */
class TCPPacket
{
public:

	TCPPacket();
	TCPPacket(TCPPacket & other);

	~TCPPacket();

	unsigned int srcPort;
	IPAddress srcIP;

	unsigned int dstPort;
	IPAddress dstIP;

	unsigned int seq;
	unsigned int ack;

	bool ackFlag;
	bool rstFlag;
	bool synFlag;

	/* Length of data of the TCP packet sent on wire */
	unsigned int length;

	/* Data of the packet may not be completely captured.
	 * This represents the length of the captured data.
	 */
	unsigned int dataLength;

	/* TCPPacket owns this data */
	const unsigned char * data;

private:
	bool parse(const unsigned char * payload, unsigned int payloadLength);

	/* Let PacketObserver parse a raw packet */
	friend class PacketObserver;

}; /* class TCPPacket */

} /* namespace packetwrapper */

#endif /* TCP_PACKET_HPP */
