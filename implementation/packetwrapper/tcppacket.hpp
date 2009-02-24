#ifndef TCP_PACKET_HPP
#define TCP_PACKET_HPP

/* Project Includes */
#include <config/config.hpp>
#include "ipaddress.hpp"
#include "packetobserver.hpp"
#include "packetinjector.hpp"

namespace packetwrapper {

/* Resolve circular dependency */
class IPAddress;
class PacketObserver;
class PacketInjector;

/* Abstraction of a TCP packet.
 * Users may inherit from this class for application-specific packets.
 */
class TCPPacket
{
public:

	TCPPacket();
	TCPPacket(TCPPacket & other);

	~TCPPacket();

	/* Assigns this packet the src and dst address and ports of other */
	void useAddress(TCPPacket & other);

	/* Assigns this packet's src and dst the dst and src address and ports of other */
	void useReverseAddress(TCPPacket & other);

	/* Makes data dataLength large, pads additional length with filling.
	 * Returns true on success, false if dataLength < the existing dataLength. */
	bool setLength(unsigned int dataLength, unsigned char filling = 0);

	unsigned short srcPort;
	IPAddress srcIP;

	unsigned short dstPort;
	IPAddress dstIP;

	unsigned int seq;
	unsigned int ack;

	bool ackFlag;
	bool pshFlag;
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

	/* Constructs a full packet with Ethernet, IP, and TCP headers.
	 * Returned buffer has length FULL_HEADER_SIZE + dataLength.
	 * Ownership of returned buffer is transferred to the user.
	 */
	unsigned char * construct();

private:
	bool parse(const unsigned char * payload, unsigned int payloadLength);

	/* Let PacketObserver parse a raw packet */
	friend class PacketObserver;
	friend class PacketInjector;

}; /* class TCPPacket */

} /* namespace packetwrapper */

#endif /* TCP_PACKET_HPP */
