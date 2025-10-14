/*
 * NflowDump.h
 *
 *  Created on: 18.03.2011
 *      Author: wocson
 */
#ifndef NFLOWDUMP_H_
#define NFLOWDUMP_H_

#include <fstream>

#include "NflowProc.h"
#include "FilterProc.h"
#include <wl/utils/Srvfunc.h>

class NflowDump: public mt::Logable
{
public:
	NflowDump(const mt::XmlConfigPtr& options);
	virtual ~NflowDump();
	void DropData(const NetFlowPack_v5& nflowpack);

private:
	std::fstream m_fileDumpNf;
};

#endif /* NFLOWDUMP_H_ */
