#ifndef CONFIG_HPP
#define CONFIG_HPP

/* Configuration and Setup */
#define PCAP_FILTER_DEFAULT	("ip and tcp")
#define ETHERNET_HEADER_TYPE	(0x0800)
#define TCP_FLAG_BIT_ACK	(1 << 4)
#define TCP_FLAG_BIT_RST	(1 << 2)
#define TCP_FLAG_BIT_SYN	(1 << 1)

/* Project Includes */
#include "debug.hpp"

#endif /* CONFIG_HPP */
