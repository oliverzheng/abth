#include <packetwrapper/packetobserver.hpp>
#include <packetwrapper/interface.hpp>

#include <iostream>

using namespace packetwrapper;
using namespace std;

int main()
{
	try {
		list<Interface> ifs = Interface::listInterfaces();
		for (list<Interface>::iterator it = ifs.begin(); it != ifs.end(); it++)
			cout << (*it).name << " (" << (*it).description << ")" << endl;
	} catch (...) {
	}

	return 0;
}
