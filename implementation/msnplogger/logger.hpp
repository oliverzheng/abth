#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <config/config.hpp>

#include <packetwrapper/packetobservable.hpp>
#include <packetwrapper/tcppacket.hpp>

namespace logger {

class Logger : virtual public packetwrapper::PacketObservable
{
public:
	Logger();
	virtual ~Logger();

protected:
	virtual void packetReceived(packetwrapper::TCPPacket * tcpPacket);

}; /* class Logger */

} /* namespace logger */

#endif
