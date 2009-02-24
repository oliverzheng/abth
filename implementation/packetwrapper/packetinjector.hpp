#ifndef PACKET_INJECTOR_HPP
#define PACKET_INJECTOR_HPP

/* Project Includes */
#include <config/config.hpp>
#include "exceptions.hpp"
#include "interfaceapp.hpp"
#include "tcppacket.hpp"

namespace packetwrapper {

/* Resolve circular dependency */
class TCPPacket;

class PacketInjector
{
public:
	virtual bool inject(TCPPacket & tcpPacket) throw(PacketInjectException) = 0;

}; /* class PacketInjector */

} /* namespace packetwrapper */

#endif /* PACKET_INJECTOR_HPP */
