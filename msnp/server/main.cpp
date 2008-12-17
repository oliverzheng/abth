/* Project Includes */
#include "mixerserver.hpp"

/* Additional Library Includes */
#include <boost/asio.hpp>

/* Standard Library Includes */
#include <iostream>
#include <string>

using namespace mixer;
using namespace boost::asio;
using namespace std;

int main(int argc, char * argv[])
{
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <email to impersonate> <name to impersonate>" << endl;
		return -1;
	}

	try {
		io_service io;
		MixerServer server(io, argv[1], argv[2]);
		io.run();
	} catch (exception & e) {
		cerr << e.what() << endl;
	}

	return 0;
}
