#include "interfaceaddress.hpp"

using namespace packetwrapper;
using namespace std;

InterfaceAddress::InterfaceAddress(string addr, string netmask, string broadaddr, string dstaddr)
	: addr(addr), netmask(netmask), broadaddr(broadaddr), dstaddr(dstaddr)
{
}
