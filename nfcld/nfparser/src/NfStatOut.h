/*
 * NfStatOut.h
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#ifndef NFSTATOUT_H_
#define NFSTATOUT_H_

#include <fstream>

#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>

class NfStatOut: public mt::Logable {
public:
	NfStatOut(const mt::XmlConfigPtr& options);
	virtual ~NfStatOut();

	void regPackets(uint32_t bytes);
	void regFlows(uint32_t flows);
	void regVradiusReq(void);
	void regVradiusSussedResp(void);
	void regVradiusErrorResp(void);
	void regTemplCmdSend(void);
	void outStats(void);

private:

	time_t m_timeOut;
	time_t m_currenttime;
	time_t m_savetime;

	uint32_t m_countpackets;
	uint32_t m_countflows;
	uint32_t m_countVradiusReq;
	uint32_t m_countVradiusSussedResp;
	uint32_t m_countVradiusErrorResp;
	uint32_t m_countTemplCmdSend;
	uint32_t m_countrecvBytes;

	std::fstream m_fileStat;

	void resetCounts(void);
};

#endif /* NFSTATOUT_H_ */
