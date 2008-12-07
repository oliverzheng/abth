#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP

/* Project Includes */
#include <config/config.hpp>

/* Additional Library Includes */
#include <boost/regex.hpp>

/* Standard Library Includes */
#include <string>
#include <ostream>

namespace packetwrapper {

/* Representation of an IPv4 address. E.g. 192.168.0.1. */
class IPAddress
{
public:
	IPAddress();

	/* Retrieve IP as string. Empty string denotes it has not been set. */
	std::string get() const;

	/* Retrieve IP as a C string. Empty string denotes it has not been set. */
	const char * getCStr() const;

	/* Retrieve IP as a 32-bit binary. E.g. 255.255.255.0 is 0xFFFFFF00. */
	unsigned int getRaw() const;

	/* Set the IP address with a string of type x.x.x.x.
	 * The validity of the IP string is checked. The IP address is only set if it is an actual IP.
	 * The return value indicates whether the IP was set.
	 */
	bool set(std::string ipAddress);

	/* Set the IP address with a 32-bit binary. */
	void set(unsigned int ipAddress);

private:
	static const boost::regex ipRegex;
	static const boost::regex digitRegex;

	/* Two representations of the IP address are used for efficiency.
	 * All operations should keep them synchronized.
	 * */
	std::string ipAddress;
	unsigned int ipAddressRaw;

}; /* class IPAddress */

} /* namespace packetwrapper */

/* For printing the IP address nicely to an ostream like std::cout. */
std::ostream & operator << (std::ostream & os, const packetwrapper::IPAddress & ipAddress);

#endif /* IP_ADDRESS_HPP */
