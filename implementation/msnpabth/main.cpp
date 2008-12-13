#include <packetwrapper/packetobserver.hpp>
#include <packetwrapper/interface.hpp>
#include "msnpabth.hpp"
#include "msnppacket.hpp"

#include <iostream>

using namespace packetwrapper;
using namespace msnpabth;
using namespace std;

int main()
{
	list<Interface> * ifs = NULL;

	try {
		ifs = new list<Interface>(Interface::listInterfaces());
	} catch (ListInterfacesException & e) {
		cerr << "Cannot list all interfaces" << endl;
		return -1;
	}

	int totalInterfaces = 0;
	for (list<Interface>::iterator it = ifs->begin(); it != ifs->end(); it++, totalInterfaces++) {
		cout << totalInterfaces << ". " << (*it).name << " (" << (*it).description << ")" << endl;
		cout << "-- addr: " << (*it).addresses.addr << endl;
		cout << "-- netmask: " << (*it).addresses.netmask << endl;
		cout << "-- broadaddr: " << (*it).addresses.broadaddr << endl;
		cout << "-- dstaddr: " << (*it).addresses.dstaddr << endl;
		cout << endl;
	}

	int whichInterface;

	do {
		cout << "Enter interface #: ";
		cin >> whichInterface;

		if (whichInterface >= 0 && whichInterface < totalInterfaces)
			break;

		cout << "Invalid Interface." << endl;
	} while (true);

	list<Interface>::iterator it = ifs->begin();
	for (int i = 0; i < whichInterface; i++)
		it++;
	(*it).open(false);
	cout << "Operating on " << (*it).name << endl;

	PacketInjector * pi = new PacketInjector();
	pi->setInterface(*it);

	PacketObserver * po = new PacketObserver();
	PacketObservable * msnpabth = new MSNPABTH(pi);

	po->setInterface(*it);
	po->setObservable(msnpabth);

	po->setFilter("port 1863");

	po->start();
	po->wait();

	return 0;
}
