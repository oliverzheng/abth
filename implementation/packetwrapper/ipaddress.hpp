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

/* Resolve circular dependency */
class TCPPacket;

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
	/* Retrieves the MAC address.
	 * Returned is a static array of 6 bytes in big endian order.
	 * Ownership of data is not transferred to the user.
	 * E.g. get()[0] = 1 represents the address 1:0:0:0:0:0.
	 */
	const unsigned char * getMAC() const;

	/* Sets the MAC address to address.
	 * Assumes address points to a 6-byte array in big endian order.
	 * E.g. for 1:0:0:0:0:0, address[0] = 1.
	 */
	void setMAC(const unsigned char * macAddress);

	static const boost::regex ipRegex;
	static const boost::regex digitRegex;

	/* Two representations of the IP address are used for efficiency.
	 * All operations should keep them synchronized.
	 * */
	std::string ipAddress;
	unsigned int ipAddressRaw;

	/* Big endian ordered mac address.
	 * E.g. 1:0:0:0:0:0 corresponds to macAddress[0] = 1.
	 */
	unsigned char macAddress[6];

	friend class TCPPacket;

}; /* class IPAddress */

} /* namespace packetwrapper */

/* For printing the IP address nicely to an ostream like std::cout. */
std::ostream & operator << (std::ostream & os, const packetwrapper::IPAddress & ipAddress);

#endif /* IP_ADDRESS_HPP */
