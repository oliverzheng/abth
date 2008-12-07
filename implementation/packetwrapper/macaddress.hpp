#ifndef MAC_ADDRESS_HPP
#define MAC_ADDRESS_HPP

/* Standard Library Includes */
#include <ostream>

namespace packetwrapper {

class MACAddress
{
public:
	/* Retrieves the MAC address.
	 * Returned is a static array of 6 bytes in big endian order.
	 * Ownership of data is not transferred to the user.
	 * E.g. get()[0] = 1 represents the address 1:0:0:0:0:0.
	 */
	const unsigned char * get() const;

	/* Sets the MAC address to address.
	 * Assumes address points to a 6-byte array in big endian order.
	 * E.g. for 1:0:0:0:0:0, address[0] = 1.
	 */
	void set(const unsigned char * macAddress);

private:
	unsigned char macAddress[6];

}; /* class MACAddress */

} /* namespace packetwrapper */

/* For printing the MAC address nicely to an ostream like std::cout. */
std::ostream & operator << (std::ostream & os, const packetwrapper::MACAddress & macAddress);

#endif /* MAC_ADDRESS_HPP */
