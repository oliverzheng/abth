#include "pcap.h"
#ifndef WIN32
    #include <sys/socket.h>
    #include <netinet/in.h>
#else
    #include <winsock.h>
#endif

#include "device.hpp"

using namespace packetwrapper;
using namespace std;

list<Interface> Device::listInterfaces() throw(ListInterfacesException)
{
	/* Retrieve the device list from the local machine */
	pcap_if_t * alldevs;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs(&alldevs, errbuf) == -1)
		throw ListInterfacesException(errbuf);

	/* Parse the list */
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

	/* We don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);

	return ifs;
}
