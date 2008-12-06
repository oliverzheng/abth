#include "ipaddress.hpp"

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

ostream & operator << (ostream & os, const packetwrapper::IPAddress & ipAddress)
{
	return os << ipAddress.get();
}
