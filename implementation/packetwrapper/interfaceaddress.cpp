/* Project Includes */
#include "interfaceaddress.hpp"

/* Namespace Declarations */
using namespace packetwrapper;

InterfaceAddress::InterfaceAddress(const IPAddress & addr, const IPAddress & netmask, const IPAddress & broadaddr, const IPAddress & dstaddr)
	: addr(addr), netmask(netmask), broadaddr(broadaddr), dstaddr(dstaddr)
{
}
