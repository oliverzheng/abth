#ifndef PACKET_OBSERVABLE_HPP
#define PACKET_OBSERVABLE_HPP

namespace packetwrapper {

class PacketObservable
{
protected:
	virtual void packetReceived() = 0;

	friend class PacketObserver;

}; /* class PacketObsevable */

} /* namespace packetwrapper */

#endif
