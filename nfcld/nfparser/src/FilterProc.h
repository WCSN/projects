/*
 * FilterProc.h
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#ifndef FILTERPROC_H_
#define FILTERPROC_H_

#include <mt/logger/Logable.h>
#include <mt/common/DynamicBuffer.h>

#include "Filter.h"
#include "NflowProc.h"

class FilterProc: public mt::Logable {
public:
	FilterProc(const mt::XmlConfigPtr& filters);
	virtual ~FilterProc();

	bool check(const Nfv5_Flow& flow);

private:

	std::vector<Filter> m_filters;
};

#endif /* FILTERPROC_H_ */
