#ifndef PACKET_OBSERVER_HPP
#define PACKET_OBSERVER_HPP

#include <boost/thread.hpp>

#include "interface.hpp"
#include "packetobservable.hpp"

#include "pcap.h"

namespace packetwrapper {

class PacketObserver
{
public:
	PacketObserver();

	bool setInterface(Interface & i);
	bool isInterfaceSet();

	bool setObservable(PacketObservable * observable);
	bool isObservableSet();

	bool start() throw(InvalidInterfaceException);
	void wait();

private:
	void loop();

	pcap_t * pcapHandle;

	PacketObservable * observable;

	boost::thread * captureThread;

}; /* class PacketObserver */

} /* namespace packetwrapper */

#endif
