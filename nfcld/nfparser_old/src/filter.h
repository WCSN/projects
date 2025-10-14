/*
 * filter.h
 *
 *  Created on: 28.12.2010
 *      Author: wocson
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

// Mobitech include.

#include <vradius_client.h>
#include <Command.h>
#include <CommandSender.h>
#include <mt/common/XmlConfig.h>
#include <mt/utils/System.h>
#include <mt/logger/LoggerFactory.h>
#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>
#include <mt/tools/Application.h>



class NetAttr
{
public:
	uint32_t network;
	uint32_t netmask;
	bool op;
	NetAttr(void){network = netmask = 0; op = true;}
};

class NetAddPar
{
public:
	uint16_t val;
	bool op;
	NetAddPar(void){val = 0; op = true;}
};

class Filter: public mt::Logable
{
public:
	string iD;
	vector < NetAttr > vsrcNets;
	vector < NetAttr > vdstNets ;
	vector < NetAddPar > vDstPorts;
	vector < NetAddPar > vToss;
	vector < NetAddPar > vProts;

	NetAttr netattr;
	NetAddPar netaddpar;

public:
	Filter(void);
	bool Check(uint32_t, uint32_t, uint16_t, uint8_t, uint8_t);

private:
	bool ipSrc_ok(uint32_t);
	bool ipDst_ok(uint32_t);
	bool port_ok(uint16_t);
	bool tos_ok(uint8_t);
	bool prot_ok(uint8_t);
};

#endif /* FILTER_H_ */
