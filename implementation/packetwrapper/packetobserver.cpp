#include "packetobserver.hpp"

#include <boost/bind.hpp>

using namespace packetwrapper;
using namespace boost;

PacketObserver::PacketObserver()
	: pcapHandle(NULL), observable(NULL), captureThread(NULL)
{
}

bool PacketObserver::setInterface(Interface & i)
{
	if (!i.isOpen())
		return false;

	pcapHandle = i.pcapHandle;

	return true;
}

bool PacketObserver::isInterfaceSet()
{
	return (pcapHandle != NULL);
}

bool PacketObserver::setObservable(PacketObservable * observable)
{
	if (observable == NULL)
		return false;

	this->observable = observable;

	return true;
}

bool PacketObserver::isObservableSet()
{
	return (observable != NULL);
}

bool PacketObserver::start() throw(InvalidInterfaceException)
{
	if (pcapHandle == NULL)
		throw InvalidInterfaceException();

	if (captureThread != NULL)
		return false;

	captureThread = new thread(bind(&PacketObserver::loop, this));

	return true;
}

void PacketObserver::wait()
{
	captureThread->join();
}

void PacketObserver::loop()
{

	debug("Entering capture thread");

	pcap_pkthdr packetHeader;
	const u_char * packetData = NULL;

	while ((packetData = pcap_next(pcapHandle, &packetHeader)) != NULL) {
		debug("Captured packet at %u %6u: packet length = %u",
		      (unsigned int)packetHeader.ts.tv_sec,
		      (unsigned int)packetHeader.ts.tv_usec,
		      (unsigned int)packetHeader.len);

		observable->packetReceived();
	}

	debug("Exiting capture thread");
}
