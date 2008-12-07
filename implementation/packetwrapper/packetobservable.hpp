#ifndef PACKET_OBSERVABLE_HPP
#define PACKET_OBSERVABLE_HPP

#include "tcppacket.hpp"

namespace packetwrapper {

class PacketObservable
{
protected:
	/*
	 * Ownership of packet is assigned to implementations of PacketObservable.
	 */
	virtual void packetReceived(TCPPacket * packet) = 0;

	friend class PacketObserver;

}; /* class PacketObsevable */

} /* namespace packetwrapper */

#endif
