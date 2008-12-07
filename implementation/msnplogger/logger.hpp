#ifndef LOGGER_HPP
#define LOGGER_HPP

/* Project Includes */
#include <config/config.hpp>
#include <packetwrapper/packetobservable.hpp>
#include <packetwrapper/tcppacket.hpp>

/* An implementation of PacketObservable.
 * Logs an averaged frequency of packets received for a given client.
 */
namespace logger {

class Logger : virtual public packetwrapper::PacketObservable
{
public:
	Logger();

protected:
	virtual void packetReceived(packetwrapper::TCPPacket * tcpPacket);

}; /* class Logger */

} /* namespace logger */

#endif /* LOGGER_HPP */
