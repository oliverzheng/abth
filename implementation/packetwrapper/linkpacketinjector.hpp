#ifndef LINK_PACKET_INJECTOR_HPP
#define LINK_PACKET_INJECTOR_HPP

/* Project Includes */
#include <config/config.hpp>
#include "exceptions.hpp"
#include "interfaceapp.hpp"
#include "tcppacket.hpp"
#include "packetinjector.hpp"

namespace packetwrapper {

/* This injects a packet at the link layer (with Ethernet header).
 * It utilizes Libpcap or Winpcap.
 */
class LinkPacketInjector : public InterfaceApp, public PacketInjector
{
public:
	bool inject(TCPPacket & tcpPacket) throw(PacketInjectException);

}; /* class LinkPacketInjector */

} /* namespace packetwrapper */

#endif /* LINK_PACKET_INJECTOR_HPP */
