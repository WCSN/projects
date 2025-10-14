/*
 * Srvfunc.h
 *
 *  Created on: 22.02.2011
 *      Author: wocson
 */

#ifndef SRVFUNC_H_
#define SRVFUNC_H_

#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stddef.h>


class Srvfunc 
{
public:
	time_t getUnixTime(void);
	time_t getUptime(void);
	uint32_t getIpAddrUint32(const std::string& str);
	std::string getIpAddrDotOct (uint32_t addr);
	std::string getTime2Log(const std::string& format);
};

#endif /* SRVFUNC_H_ */


