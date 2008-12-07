/* Project Includes */
#include "macaddress.hpp"

/* Standard Library Includes */
#include <cstring>

/* Namespace Declarations */
using namespace packetwrapper;

const unsigned char * MACAddress::get() const
{
	return macAddress;
}

void MACAddress::set(const unsigned char * macAddress)
{
	memcpy(this->macAddress, macAddress, 6);
}
