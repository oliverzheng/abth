#ifndef INTERFACE_ADDRESS_HPP
#define INTERFACE_ADDRESS_HPP

#include <string>

namespace packetwrapper {

class InterfaceAddress
{
public:
	InterfaceAddress() {};
	InterfaceAddress(std::string addr,
	                 std::string netmask,
	                 std::string broadaddr,
	                 std::string dstaddr);

	std::string addr;
	std::string netmask;
	std::string broadaddr;
	std::string dstaddr;

}; /* class InterfaceAddress */

} /* namespace packewrapper */

#endif /* INTERFACE_ADDRESS_HPP */
