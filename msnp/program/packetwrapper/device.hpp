#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <list>
#include "interface.hpp"
#include "exceptions.hpp"

namespace packetwrapper {

class Device
{
public:
	static std::list<Interface> listInterfaces() throw(ListInterfacesException);

private:
	Device() {};

}; /* class Driver */

} /* namespace packewrapper */

#endif /* DEVICE_HPP */
