#ifndef PACKET_OBSERVER_HPP
#define PACKET_OBSERVER_HPP

/* Project Includes */
#include <config/config.hpp>
#include "interface.hpp"
#include "packetobservable.hpp"

/* Additional Library Includes */
#include <boost/thread.hpp>
#include <pcap.h>

namespace packetwrapper {

/* Resolve circular dependency */
class TCPPacket;
class PacketObservable;

/* Captures packets on a given interface. */
class PacketObserver
{
public:
	PacketObserver();

	/* Use an opened interface.
	 * Returns true if interface is open and will be used, otherwise false.
	 */
	bool setInterface(Interface & i);

	bool isInterfaceSet();

	/* Forward captured packets to a PacketObservable.
	 * Returns true if observable is not null and will be used, otherwise false.
	 */
	bool setObservable(PacketObservable * observable);

	bool isObservableSet();

	/* Start capturing on the interface, if it is set and the observable is also set.
	 * Launches a new thread from which event handlers of new packets will be called.
	 * Returns true if capturing has successfully been started, false otherwise.
	 */
	bool start() throw(InvalidInterfaceException, InterfaceFilterException);

	/* Blocks until the capture thread exits */
	void wait();

private:
	/* Thread entry */
	void loop();

	pcap_t * pcapHandle;

	/* IP subnet mask for which the capture filter is set.
	 * Should be retrieved from the interface being captured on.
	 */
	unsigned int netmask;

	PacketObservable * observable;

	boost::thread * captureThread;

}; /* class PacketObserver */

} /* namespace packetwrapper */

#endif /* PACKET_OBSERVER_HPP */
