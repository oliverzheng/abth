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
		list<Interface> ifs = Interface::listInterfaces();
		for (list<Interface>::iterator it = ifs.begin(); it != ifs.end(); it++)
			cout << (*it).name << " (" << (*it).description << ")" << endl;

		list<Interface>::iterator it = ifs.begin();
		it++;
		it++;
		(*it).open(false);

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
