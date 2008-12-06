#ifndef INTERFACE_ADDRESS_HPP
#define INTERFACE_ADDRESS_HPP

#include "ipaddress.hpp"

namespace packetwrapper {

class InterfaceAddress
{
public:
	InterfaceAddress() {};
	InterfaceAddress(IPAddress addr,
	                 IPAddress netmask,
	                 IPAddress broadaddr,
	                 IPAddress dstaddr);

	IPAddress addr;
	IPAddress netmask;
	IPAddress broadaddr;
	IPAddress dstaddr;

}; /* class InterfaceAddress */

} /* namespace packewrapper */

#endif /* INTERFACE_ADDRESS_HPP */
