#include "ipaddress.hpp"
#include <cstdio>

using namespace packetwrapper;
using namespace boost;
using namespace std;

const regex IPAddress::ipRegex("(\\d{1,3}\\.){3}\\d{1,3}");
const regex IPAddress::digitRegex("\\d");

IPAddress::IPAddress()
	: ipAddress(), ipAddressRaw()
{
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

	sregex_token_iterator i(ipAddress.begin(), ipAddress.end(), digitRegex, -1);
	sregex_token_iterator j;

	unsigned int ipAddressRawNew = 0;

	while(i != j) {
		int octet = atoi(string(*i++).c_str());
		if ((octet >> 8) != 0)
			return false;

		ipAddressRaw = (ipAddressRaw << 8) | octet;
	}

	ipAddressRaw = ipAddressRawNew;

	this->ipAddress = ipAddress;

	return true;
}

bool IPAddress::set(unsigned int ipAddress)
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

	return true;
}

ostream & operator << (ostream & os, const packetwrapper::IPAddress & ipAddress)
{
	return os << ipAddress.get();
}
