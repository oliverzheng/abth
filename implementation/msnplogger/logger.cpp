#include "logger.hpp"

using namespace logger;
using namespace packetwrapper;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::packetReceived(TCPPacket * tcpPacket)
{
	debug("TCP packet received: %s:%u to %s:%u (%u/%u)\n\
		seq(%u) ack(%u) ack=%u, rst=%u, syn=%u",
		tcpPacket->srcIP.getCStr(), tcpPacket->srcPort,
		tcpPacket->dstIP.getCStr(), tcpPacket->dstPort,
		tcpPacket->dataLength, tcpPacket->length,
		tcpPacket->seq, tcpPacket->ack,
		(tcpPacket->ackFlag) ? 1 : 0,
		(tcpPacket->rstFlag) ? 1 : 0,
		(tcpPacket->synFlag) ? 1 : 0);
	delete tcpPacket;
}
