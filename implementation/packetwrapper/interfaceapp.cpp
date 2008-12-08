/* Project Includes */
#include "interfaceapp.hpp"

/* Namespace Declarations */
using namespace packetwrapper;

InterfaceApp::InterfaceApp()
	: pcapHandle(NULL), netmask(0xFFFFFFFF)
{
}

InterfaceApp::~InterfaceApp()
{
}

bool InterfaceApp::setInterface(Interface & i)
{
	if (!i.isOpen())
		return false;

	pcapHandle = i.pcapHandle;

	if (!i.addresses.netmask.get().empty())
		netmask = i.addresses.netmask.getRaw();
	else
		netmask = 0xFFFFFFFF;

	return true;
}

bool InterfaceApp::isInterfaceSet()
{
	return (pcapHandle != NULL);
}
