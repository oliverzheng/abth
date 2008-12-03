#include <packetwrapper/packetobserver.hpp>
#include <packetwrapper/device.hpp>

#include <iostream>

using namespace packetwrapper;
using namespace std;

int main()
{
	list<Interface> ifs = Device::listInterfaces();
	for (list<Interface>::iterator it = ifs.begin(); it != ifs.end(); it++)
		cout << (*it).name << endl;

	PacketObserver * po = new PacketObserver(*(ifs.begin()));
	return 0;
}
