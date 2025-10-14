/*
 * Filter.h
 *
 *  Created on: 10.02.2011
 *      Author: wocson
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <mt/logger/Logable.h>
#include <mt/common/XmlConfig.h>

#include "FCriterion.h"
#include "NflowProc.h"

class Filter: public mt::Logable {
public:
	Filter(const mt::XmlConfigPtr& cfg);
	virtual ~Filter();

	bool check(const Nfv5_Flow& flow) const;
	std::string getFilterId(void) const {
		return m_filterId;
	}
	;
private:
	std::vector<FCriterion*> m_fcriterions;
	std::string m_filterId;

	Filter(void);
};

#endif /* FILTER_H_ */
