#ifndef PACKET_OBSERVER_HPP
#define PACKET_OBSERVER_HPP

/* Project Includes */
#include <config/config.hpp>
#include "interface.hpp"
#include "packetobservable.hpp"
#include "interfaceapp.hpp"

/* Additional Library Includes */
#include <boost/thread.hpp>
#include <pcap.h>

/* Standard Library Includes */
#include <string>

namespace packetwrapper {

/* Resolve circular dependency */
class Interface;
class PacketObservable;

/* Captures packets on a given interface. */
class PacketObserver : public InterfaceApp
{
public:
	PacketObserver();

	/* Forward captured packets to a PacketObservable.
	 * Returns true if observable is not null and will be used, otherwise false.
	 */
	bool setObservable(PacketObservable * observable);

	bool isObservableSet();

	void setFilter(std::string filterString) throw(InvalidInterfaceException, InterfaceFilterException);

	/* Start capturing on the interface, if it is set and the observable is also set.
	 * Launches a new thread from which event handlers of new packets will be called.
	 * Returns true if capturing has successfully been started, false otherwise.
	 */
	bool start() throw(InvalidInterfaceException, InterfaceFilterException);

	/* Exits the capture thread */
	void stop();

	/* Blocks until the capture thread exits */
	void wait();

private:
	static const char * pcapDefaultFilter;
	std::string appFilter;

	/* Thread entry */
	void loop();

	PacketObservable * observable;

	boost::thread * captureThread;

	bool running;

}; /* class PacketObserver */

} /* namespace packetwrapper */

#endif /* PACKET_OBSERVER_HPP */
