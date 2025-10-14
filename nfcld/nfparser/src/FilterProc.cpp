/*
 * FilterProc.cpp
 *
 *  Created on: 11.02.2011
 *      Author: wocson
 */

#include "FilterProc.h"
#include <wl/utils/Srvfunc.h>

FilterProc::FilterProc(const mt::XmlConfigPtr& filters) {
	MT_LOGINIT(FilterProc);
	MT_LOGTRACE("Init Filters.");
	mt::ConfigList subflt = filters->getConfigs("Filter");

	for (mt::ConfigList::iterator it = subflt.begin(); it != subflt.end(); it++) {
		m_filters.push_back(Filter((*it)));
	}
}

FilterProc::~FilterProc() {

}

bool FilterProc::check(const Nfv5_Flow& flow) {

	MT_LOGTRACE("Checked Filters on flow.");
	std::vector<Filter>::const_iterator itfilter;

	for (itfilter = m_filters.begin(); itfilter != m_filters.end(); itfilter++) {
		if ((*itfilter).check(flow)) {
			MT_LOGINFO("Find IP:  " << Srvfunc().getIpAddrDotOct(flow.srcaddr) << " Filter id: " << (*itfilter).getFilterId());
			return true;
		}
	}
	return false;
}
