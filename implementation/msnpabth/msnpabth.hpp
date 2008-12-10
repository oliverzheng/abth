#ifndef MSNP_ABTH_HPP
#define MSNP_ABTH_HPP

/* Project Includes */
#include <config/config.hpp>
#include <packetwrapper/packetobservable.hpp>

namespace msnpabth {

class MSNPABTH : public packetwrapper::PacketObservable
{
public:

protected:
	virtual void packetReceived(packetwrapper::TCPPacket * tcpPacket);

}; /* class MSNPABTH */

} /* namespace msnpabth */

#endif /* MSNP_ABTH_HPP */
