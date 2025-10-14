/*
 * FCriterion.h
 *
 *  Created on: 10.02.2011
 *      Author: wocson
 */

#ifndef FCRITERION_H_
#define FCRITERION_H_

#include <mt/logger/Logable.h>
#include <mt/common/XmlConfig.h>
#include "NfReceiver.h"
#include "NflowProc.h"

enum Operation {EXCLUDE = false, INCLUDE = true};

class FCriterion: public mt::Logable {
protected:
	FCriterion();
	FCriterion(const mt::XmlConfigPtr& cfg);
	virtual ~FCriterion() {	}

	Operation m_operation;

	virtual bool isCriterionApplied(const Nfv5_Flow& flow) = 0;
public:
	bool check(const Nfv5_Flow& flow);
};

class FCriterionSrcNet: public FCriterion {
private:
	uint32_t m_ui32addr;
	uint32_t m_ui32mask;

public:
	FCriterionSrcNet(const mt::XmlConfigPtr& cfg);
	virtual ~FCriterionSrcNet();

	bool isCriterionApplied(const Nfv5_Flow& flow);
};

class FCriterionDstNet: public FCriterion {
private:
	uint32_t m_ui32addr;
	uint32_t m_ui32mask;

public:
	//FCriterionSrcNet();
	FCriterionDstNet(const mt::XmlConfigPtr& cfg);
	virtual ~FCriterionDstNet();

	bool isCriterionApplied(const Nfv5_Flow& flow);
};

class FCriterionPort: public FCriterion {
private:
	uint16_t m_ui16port;
public:
	FCriterionPort(const mt::XmlConfigPtr& cfg);
	virtual ~FCriterionPort();

	bool isCriterionApplied(const Nfv5_Flow& flow);
};

class FCriterionProtocol: public FCriterion {
private:
	uint8_t m_ui8protocol;
public:
	FCriterionProtocol(const mt::XmlConfigPtr& cfg);
	virtual ~FCriterionProtocol();

	bool isCriterionApplied(const Nfv5_Flow& flow);
};

class FCriterionTos: public FCriterion {
private:
	uint8_t m_ui8tos;
public:
	FCriterionTos(const mt::XmlConfigPtr& cfg);
	virtual ~FCriterionTos();

	bool isCriterionApplied(const Nfv5_Flow& flow);
};

#endif /* FCRITERION_H_ */
