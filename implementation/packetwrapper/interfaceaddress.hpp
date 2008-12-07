#ifndef INTERFACE_ADDRESS_HPP
#define INTERFACE_ADDRESS_HPP

/* Project Includes */
#include "ipaddress.hpp"

namespace packetwrapper {

/* A set of IP addresses utilized by each network interface */
class InterfaceAddress
{
public:
	InterfaceAddress() {};
	InterfaceAddress(const IPAddress & addr,
	                 const IPAddress & netmask,
	                 const IPAddress & broadaddr,
	                 const IPAddress & dstaddr);

	/* Each of these may have empty IP addresses, indicating lack of an address. */
	const IPAddress addr;
	const IPAddress netmask;
	const IPAddress broadaddr;
	const IPAddress dstaddr;

}; /* class InterfaceAddress */

} /* namespace packewrapper */

#endif /* INTERFACE_ADDRESS_HPP */
