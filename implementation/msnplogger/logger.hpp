#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "packetwrapper/packetobservable.hpp"

namespace logger {

class Logger : virtual public packetwrapper::PacketObservable
{
public:
	Logger();
	virtual ~Logger();

protected:
	virtual void packetReceived();

}; /* class Logger */

} /* namespace logger */

#endif
