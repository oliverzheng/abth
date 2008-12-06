#include "interface.hpp"

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock.h>
#endif

using namespace packetwrapper;
using namespace std;

Interface::Interface(string name, string description, InterfaceAddress addresses)
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

bool Interface::isOpen()
{
	return (pcapHandle != NULL);
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
		IPAddress addr, netmask, broadaddr, dstaddr;
		for (pcap_addr * a = d->addresses; a != NULL; a = a->next) {
			if (a->addr != NULL)
				addr.set(inet_ntoa(((struct sockaddr_in *)a->addr)->sin_addr));
			if (a->netmask != NULL)
				netmask.set(inet_ntoa(((struct sockaddr_in *)a->netmask)->sin_addr));
			if (a->broadaddr != NULL)
				broadaddr.set(inet_ntoa(((struct sockaddr_in *)a->broadaddr)->sin_addr));
			if (a->dstaddr != NULL)
				dstaddr.set(inet_ntoa(((struct sockaddr_in *)a->dstaddr)->sin_addr));
		}

		ifs.push_back(Interface(d->name, d->description, InterfaceAddress(addr,
										  netmask,
										  broadaddr,
										  dstaddr)));
	}

	pcap_freealldevs(alldevs);

	return ifs;
}
