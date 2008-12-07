#ifndef PACKET_OBSERVER_HPP
#define PACKET_OBSERVER_HPP

#include <config/config.hpp>

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

	bool start() throw(InvalidInterfaceException, InterfaceFilterException);
	void wait();

private:
	void loop();

	pcap_t * pcapHandle;
	unsigned int netmask;

	PacketObservable * observable;

	boost::thread * captureThread;

}; /* class PacketObserver */

} /* namespace packetwrapper */

#endif
