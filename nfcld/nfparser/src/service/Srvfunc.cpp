/*
 * Srvfunc.cpp
 *
 *  Created on: 22.02.2011
 *      Author: wocson
 */

#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "Srvfunc.h"

uint32_t Srvfunc::getIpAddrUint32(const std::string& str)
{
	sockaddr_in net_address;
	inet_aton(str.c_str(), &net_address.sin_addr); // IP as string to uint
	return htonl(net_address.sin_addr.s_addr); // get IP as number uint32_t for src_net;
}

std::string Srvfunc::getIpAddrDotOct(uint32_t addr)
{
	//	in_addr address;
	static std::string Ipaddr;
	char ipaddr[sizeof("255.255.255.255") + 1];

	//	address.s_addr = htonl(addr);
	//	strcpy(ipaddr,	inet_ntoa(address));

	uint32_t saddr = htonl(addr);

	sprintf(ipaddr, "%u.%u.%u.%u", (unsigned char) (saddr),
			(unsigned char) (saddr >> 8),
			(unsigned char) (saddr >> 16),
			(unsigned char) (saddr >> 24));

	return Ipaddr.assign(ipaddr);
}

time_t Srvfunc::getUnixTime(void)
{
	static time_t tt;
	time(&tt); /* Get datetime in seconds */
	return tt;
}

//"%Y-%m-%d %H:%M:%S"    	//2010-01-13 00:00:01
std::string Srvfunc::getTime2Log(const std::string& format)
{
	time_t tt;
	struct tm *ts;
	char timeL[128];

	tzset();
	time(&tt);
	ts = localtime(&tt);

	strftime(timeL, 512, format.c_str(), ts);

	return std::string(timeL);
}

time_t Srvfunc::getUptime(void)
{
	time_t boot_time;
    static int request[2] = { CTL_KERN, KERN_BOOTTIME };
    struct timeval result;

    size_t result_len = sizeof result;

    if (sysctl (request, 2, &result, &result_len, NULL, 0) >= 0)
    	boot_time = result.tv_sec;

return boot_time;
}
