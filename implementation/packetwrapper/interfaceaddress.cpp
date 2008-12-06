#include "interfaceaddress.hpp"

using namespace packetwrapper;

InterfaceAddress::InterfaceAddress(IPAddress addr, IPAddress netmask, IPAddress broadaddr, IPAddress dstaddr)
	: addr(addr), netmask(netmask), broadaddr(broadaddr), dstaddr(dstaddr)
{
}
