/* Project Includes */
#include "ipaddress.hpp"

/* Standard Library Includes */
#include <cstdio>
#include <cstring>

/* Namespace Declarations */
using namespace packetwrapper;
using namespace boost;
using namespace std;

/* Static Const Member Declarations */
const regex IPAddress::ipRegex("(\\d{1,3}\\.){3}\\d{1,3}");
const regex IPAddress::delimiterRegex("\\.");

IPAddress::IPAddress()
	: ipAddress(), ipAddressRaw()
{
	memset(macAddress, 0, sizeof(macAddress));
}

IPAddress & IPAddress::operator=(const IPAddress & other)
{
	if (this != &other) {
		ipAddress = other.ipAddress;
		ipAddressRaw = other.ipAddressRaw;
		memcpy(macAddress, other.macAddress, sizeof(macAddress));
	}

	return *this;
}

bool IPAddress::operator==(const IPAddress& other) const {
	if ( this->ipAddress.compare( other.ipAddress ) == 0 ) {
		return true;
	}

	return false;
}

string IPAddress::get() const
{
	return ipAddress;
}

const char * IPAddress::getCStr() const
{
	return ipAddress.c_str();
}

unsigned int IPAddress::getRaw() const
{
	return ipAddressRaw;
}

bool IPAddress::set(string ipAddress)
{
	if (!regex_match(ipAddress, ipRegex))
		return false;

	sregex_token_iterator i(ipAddress.begin(), ipAddress.end(), delimiterRegex, -1);
	sregex_token_iterator j;

	unsigned int ipAddressRawNew = 0;

	while(i != j) {
		int octet = atoi(string(*i++).c_str());
		if ((octet >> 8) != 0)
			return false;

		ipAddressRawNew = (ipAddressRawNew << 8) | octet;
	}

	ipAddressRaw = ipAddressRawNew;

	this->ipAddress = ipAddress;

	return true;
}

void IPAddress::set(unsigned int ipAddress)
{
	ipAddressRaw = ipAddress;

	this->ipAddress = "";

	char part[4];
	for (int i = 3; i >= 0; i--) {
		sprintf(part, "%d", (ipAddress & (0xFF << (i * 8))) >> (i * 8));
		this->ipAddress += part;
		if (i != 0)
			this->ipAddress += ".";
	}
}

const unsigned char * IPAddress::getMAC() const
{
	return macAddress;
}

void IPAddress::setMAC(const unsigned char * macAddress)
{
	memcpy(this->macAddress, macAddress, sizeof(this->macAddress));
}

ostream & operator << (ostream & os, const packetwrapper::IPAddress & ipAddress)
{
	return os << ipAddress.get();
}
