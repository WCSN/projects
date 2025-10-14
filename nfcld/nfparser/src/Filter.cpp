/*
 * Filter.cpp
 *
 *  Created on: 10.02.2011
 *      Author: wocson
 */

#include "FCriterion.h"
#include "Filter.h"

Filter::Filter(const mt::XmlConfigPtr& filter) {
	MT_LOGINIT(Filter);

	m_filterId = filter->get("id");

	mt::ConfigList::iterator it;
	mt::ConfigList subflt;
	MT_LOGTRACE("InitCriterions");

	subflt = filter->getConfigs("srcNet");
	for (it = subflt.begin(); it != subflt.end(); it++)
		m_fcriterions.push_back(new FCriterionSrcNet(*it));

	subflt = filter->getConfigs("dstNet");
	for (it = subflt.begin(); it != subflt.end(); it++)
		m_fcriterions.push_back(new FCriterionDstNet(*it));

	subflt = filter->getConfigs("dstport");
	for (it = subflt.begin(); it != subflt.end(); it++)
		m_fcriterions.push_back(new FCriterionPort(*it));

	subflt = filter->getConfigs("protocol");
	for (it = subflt.begin(); it != subflt.end(); it++)
		m_fcriterions.push_back(new FCriterionProtocol(*it));

	subflt = filter->getConfigs("typeos");
	for (it = subflt.begin(); it != subflt.end(); it++)
		m_fcriterions.push_back(new FCriterionTos(*it));

}

Filter::~Filter() {

}

bool Filter::check(const Nfv5_Flow& flow) const {

	MT_LOGTRACE("Check Filter criterions Filter id: "<< m_filterId);
	bool st_check = false;
	std::vector<FCriterion*>::const_iterator iCriterion;

	for (iCriterion = m_fcriterions.begin(); iCriterion != m_fcriterions.end(); iCriterion++)
	{
		//st_check = (*iCriterion)->isCriterionApplied(flow);
	    st_check = (*iCriterion)->check(flow);
	    if(st_check)
	    	break;
	}

	return st_check;
}
