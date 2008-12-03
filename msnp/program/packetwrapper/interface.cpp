#include "interface.hpp"

using namespace packetwrapper;
using namespace std;

Interface::Interface(string name, string description, list<Address> addresses)
	: name(name), description(description), addresses(addresses)
{
}
