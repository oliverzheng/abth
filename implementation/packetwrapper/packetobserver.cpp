/* Project Includes */
#include "packetobserver.hpp"

/* Additional Library Includes */
#include <boost/bind.hpp>

/* Namespace Declarations */
using namespace packetwrapper;
using namespace boost;

PacketObserver::PacketObserver()
	: observable(NULL), captureThread(NULL)
{
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

	/* Capture only TCP/IP packets with the libpcap filter */
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
	if (captureThread != NULL)
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
