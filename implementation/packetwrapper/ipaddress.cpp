#include "ipaddress.hpp"
#include <cstdio>

using namespace packetwrapper;
using namespace boost;
using namespace std;

const regex IPAddress::ipRegex("(\\d{1,3}\\.){3}\\d{1,3}");

IPAddress::IPAddress()
	: ipAddress()
{
}

string IPAddress::get() const
{
	return ipAddress;
}

bool IPAddress::set(string ipAddress)
{
	if (!regex_match(ipAddress, ipRegex))
		return false;

	this->ipAddress = ipAddress;

	return true;
}

bool IPAddress::set(unsigned int ipAddress)
{
	this->ipAddress = "";

	char part[4];
	for (int i = 0; i <= 3; i++) {
		sprintf(part, "%d", (ipAddress & (0x000F << (i * 4))) >> (i * 4));
		this->ipAddress += part;
		if (i != 3)
			this->ipAddress += ".";
	}

	return true;
}

ostream & operator << (ostream & os, const packetwrapper::IPAddress & ipAddress)
{
	return os << ipAddress.get();
}
