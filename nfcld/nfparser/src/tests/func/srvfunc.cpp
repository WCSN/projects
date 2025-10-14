/*
 * srvfunc.cpp
 *
 *  Created on: 01.02.2011
 *      Author: wocson
 */

#include <mt/common/DynamicBuffer.h>
#include "srvfunc.h"


int start_nfparser(const std::string& conf)
{
	std::string cmd;
	//cmd = "./nfparser_old -d -c \"../../nfparser/etc/test_configs/" + conf + "\" >> ./nfparser.log &";
	cmd = "./nfparser -c \"../../nfparser/etc/test_configs/" + conf + "\" >> ./nfparser.log &";

	//std::cout <<"\ncmd="<<cmd.c_str()<<"\n";
	int retsystem = system(cmd.c_str());
	sleep(1);
return retsystem;
}

int stop_nfparser(void)
{
//	int retsystem = system("killall -KILL nfparser_old");
	int retsystem = system("killall -KILL nfparser");
	sleep(1);
return retsystem;
}


void outbytes(std::string name, mt::ByteBuffer buffout)
{
		std::cout << name <<": ";

		for(uint16_t i=0; i < buffout.getSize(); i++)
		{
			std::cout <<  std::hex << (uint16_t)buffout[i];
		}
			std::cout << "\n";
}
