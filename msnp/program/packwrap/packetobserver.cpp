#include <iostream>
#include <boost/thread.hpp>

#include "packetobserver.hpp"

using namespace std;
using namespace boost;

static void abcd()
{
	cout << "test" << endl;
}

void test()
{
	thread * testThread = new thread(test);
	testThread->join();
}

/*int main()
{
}
*/
