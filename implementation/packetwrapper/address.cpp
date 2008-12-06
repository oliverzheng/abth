#include "address.hpp"

using namespace packetwrapper;
using namespace std;

Address::Address(string addr, string netmask, string broadaddr, string dstaddr)
	: addr(addr), netmask(netmask), broadaddr(broadaddr), dstaddr(dstaddr)
{
}
