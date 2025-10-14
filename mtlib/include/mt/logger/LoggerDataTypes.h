/*
 * LoggerDataTypes.h
 *
 *  Created on: 13.08.2010
 *      Author: hakamasy
 */

#ifndef LOGGERDATATYPES_H_
#define LOGGERDATATYPES_H_

#include "mt/common/SmartPtr.h"
#include <vector>
#include <map>
#include <string>


namespace mt {

class Logger;
typedef SmartPtr<Logger> LoggerPtr;


class LogCollector;
typedef SmartPtr<LogCollector> LogCollectorPtr;

enum LogLevel {
	TRACE = 0,
	DEBUG = 1,
	INFO = 2,
	WARNING = 3,
	ERROR = 4,
	CRITICAL = 5,
	REWRITE = 6
};


}


#endif /* LOGGERDATATYPES_H_ */
