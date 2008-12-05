#include <packetwrapper/packetobserver.hpp>
#include <packetwrapper/interface.hpp>
#include "logger.hpp"

#include <iostream>

using namespace packetwrapper;
using namespace logger;
using namespace std;

int main()
{
	try {
		int totalInterfaces = 0;
		list<Interface> ifs = Interface::listInterfaces();
		for (list<Interface>::iterator it = ifs.begin(); it != ifs.end(); it++, totalInterfaces++) {
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
			cout << endl;
	
			if (whichInterface >= 0 && whichInterface < totalInterfaces)
				break;

			cout << "Invalid Interface." << endl;
		} while (true);

		list<Interface>::iterator it = ifs.begin();
		for (int i = 0; i < whichInterface; i++)
			it++;
		(*it).open(false);
		cout << "Listening on " << (*it).name << endl;

		PacketObserver * po = new PacketObserver();
		PacketObservable * logger = new Logger();

		po->setInterface(*it);
		po->setObservable(logger);

		po->start();
		po->wait();

	} catch (...) {
		cout << "what happened here?" << endl;
	}

	return 0;
}
