/*
 * Logable.h
 *
 *  Created on: 18.07.2010
 *      Author: hakamasy
 */

#ifndef LOGABLE_H_
#define LOGABLE_H_

#include "mt/logger/Logger.h"

namespace mt {

/**
 * \class Logable
 *
 * \brief if you want enable
 */


class Logable {

protected:

	LoggerPtr m_logger;
	std::string m_loggerPrefix;
public:
	Logable();
	void setLoggerPrefix(const std::string &prefix){
		MT_LOG_SETPREFIX(prefix)
	}
};

}
#endif /* LOGABLE_H_ */
