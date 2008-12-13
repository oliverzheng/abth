#ifndef PACKET_OBSERVABLE_HPP
#define PACKET_OBSERVABLE_HPP

/* Project Includes */
#include "tcppacket.hpp"
#include "packetobserver.hpp"

namespace packetwrapper {

/* Resolve circular dependency */
class PacketObserver;
class TCPPacket;

/* An interface to which captured packets will be sent. */
class PacketObservable
{
public:
	virtual ~PacketObservable() = 0;

protected:
	/* Ownership of packet is not transfered to the implementation of PacketObservable. */
	virtual void packetReceived(TCPPacket & packet) = 0;

	/* Let Packetobserver call packetReceived. */
	friend class PacketObserver;

}; /* class PacketObsevable */

} /* namespace packetwrapper */

#endif /* PACKET_OBSERVAVBLE_HPP */
