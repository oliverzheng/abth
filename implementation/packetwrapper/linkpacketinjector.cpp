/* Project Includes */
#include "linkpacketinjector.hpp"
#include "networkheaders.hpp"

/* Additional Library Includes */
#include <pcap.h>

/* Namespace Declarations */
using namespace packetwrapper;

bool LinkPacketInjector::inject(TCPPacket & tcpPacket) throw(PacketInjectException)
{
	if (!isInterfaceSet())
		return false;

	unsigned char * packet = tcpPacket.construct();

	int packetLength = FULL_HEADER_SIZE + tcpPacket.dataLength;

	if (pcap_sendpacket(pcapHandle, packet, packetLength) < 0)
		throw PacketInjectException(pcap_geterr(pcapHandle));

	delete packet;

	return true;
}
