#ifndef INTERFACE_ADDRESS_HPP
#define INTERFACE_ADDRESS_HPP

#include "ipaddress.hpp"

namespace packetwrapper {

class InterfaceAddress
{
public:
	InterfaceAddress() {};
	InterfaceAddress(const IPAddress & addr,
	                 const IPAddress & netmask,
	                 const IPAddress & broadaddr,
	                 const IPAddress & dstaddr);

	const IPAddress addr;
	const IPAddress netmask;
	const IPAddress broadaddr;
	const IPAddress dstaddr;

}; /* class InterfaceAddress */

} /* namespace packewrapper */

#endif /* INTERFACE_ADDRESS_HPP */
