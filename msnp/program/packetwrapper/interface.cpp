#include "interface.hpp"
#include <iostream>

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
#else
	#include <winsock.h>
#endif

using namespace packetwrapper;
using namespace std;

Interface::Interface(string name, string description, list<Address> addresses)
	: name(name), description(description), addresses(addresses), pcapHandle(NULL)
{
}

Interface::Interface(const Interface & i)
	: name(i.name), description(i.description), addresses(i.addresses), pcapHandle(NULL)
{
}

void Interface::open(bool promiscuous) throw(OpenInterfaceException)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((pcapHandle = pcap_open_live(name.c_str(),
	                                 65535,
	                                 (promiscuous) ? 1 : 0,
					 0,
					 errbuf)) == NULL) {
		throw OpenInterfaceException(errbuf);
	}
}

/* Static Member Methods */

list<Interface> Interface::listInterfaces() throw(ListInterfacesException)
{
	pcap_if_t * alldevs;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs(&alldevs, errbuf) == -1)
		throw ListInterfacesException(errbuf);

	list<Interface> ifs;
	
	for (pcap_if_t * d = alldevs; d != NULL; d = d->next) {
		list<Address> addrs;

		/* FIXME: convert these to IP addresses
		for (pcap_addr * a = d->addresses; a != NULL; a = a->next)
			addrs.push_back(Address((a->addr) ? iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr) : NULL,
			                        (a->netmask) ? a->netmask->sa_data : NULL,
			                        (a->broadaddr) ? a->broadaddr->sa_data : NULL,
			                        (a->dstaddr) ? a->dstaddr->sa_data : NULL));
		*/
		ifs.push_back(Interface(d->name, d->description, addrs));
	}

	pcap_freealldevs(alldevs);

	return ifs;
}
