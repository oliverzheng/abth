/* Project Includes */
#include "logger.hpp"
#include <string>

/* Additional Library Includes */
#include <boost/regex.hpp>

/* Namespace Declarations */
using namespace logger;
using namespace boost;
using namespace boost::posix_time;
using namespace msnp;
using namespace packetwrapper;

Logger::Timer::Timer(time_duration duration)
	: logDuration(duration)
{
	resetTimer();
}

void Logger::Timer::resetTimer()
{
	endTime = microsec_clock::local_time() + logDuration;
}

bool Logger::Timer::isTimerExpired()
{
	ptime curTime = microsec_clock::local_time();

	if ( curTime < endTime ) {
		return false;
	}
	return true;
}

Logger::Logger()
: numContacts(0)
{
	contactManager = new MSNPContactManager();
	timer = new Timer(seconds(Timer::ABTH_DURATION_SEC));
}

void Logger::packetReceived(TCPPacket & tcpPacket)
{
	// disregard all empty packets
	if (tcpPacket.dataLength == 0) return;

	MSNPPacket msnpPacket(tcpPacket);

	contactManager->ParsePacket(msnpPacket);

	static bool start = false;
	if ( start ) {
		if ( timer->isTimerExpired() ) {
			Log("good");
		} else {
			Log("abth gone awry");
		}

		start = false;
	}
	
	if ( msnpPacket.getCommand() == msnpPacket.PING_RESPONSE ) {
		timer->resetTimer();
		start = true;
	}
}

void Logger::Log (std::string msg) 
{
	static unsigned int cnt = 1;
	ptime curTime = second_clock::local_time();

	std::cout << cnt++ << "\t" << 
		curTime.date() << "\t" <<
		curTime.time_of_day() << "\t" << 
		contactManager->GetNumOnline() << "\t" <<
		msg << "\t" <<
		std::endl;
}
