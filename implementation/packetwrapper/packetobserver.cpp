#include "packetobserver.hpp"

#include <boost/bind.hpp>

using namespace packetwrapper;
using namespace boost;

PacketObserver::PacketObserver()
	: pcapHandle(NULL), netmask(0xFFFFFFFF), observable(NULL), captureThread(NULL)
{
}

bool PacketObserver::setInterface(Interface & i)
{
	if (!i.isOpen())
		return false;

	pcapHandle = i.pcapHandle;

	if (!i.addresses.netmask.get().empty())
		netmask = i.addresses.netmask.getRaw();
	else
		netmask = 0xFFFFFFFF;

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

bool PacketObserver::start() throw(InvalidInterfaceException, InterfaceFilterException)
{
	if (pcapHandle == NULL)
		throw InvalidInterfaceException();

	if (captureThread != NULL)
		return false;

	struct bpf_program filter;

	if (pcap_compile(pcapHandle, &filter, PCAP_FILTER_DEFAULT, 1, netmask) < 0)
		throw InterfaceFilterException(pcap_geterr(pcapHandle));

	if (pcap_setfilter(pcapHandle, &filter) < 0)
		throw InterfaceFilterException(pcap_geterr(pcapHandle));

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
	const unsigned char * packetData = NULL;

	while ((packetData = pcap_next(pcapHandle, &packetHeader)) != NULL) {
		debug("Captured packet at %u %6u: packet length = %u",
		      (unsigned int)packetHeader.ts.tv_sec,
		      (unsigned int)packetHeader.ts.tv_usec,
		      (unsigned int)packetHeader.len);

		TCPPacket * tcpPacket = new TCPPacket();
		tcpPacket->parse(packetData, packetHeader.caplen);

		observable->packetReceived(tcpPacket);
	}

	debug("Exiting capture thread");
}
