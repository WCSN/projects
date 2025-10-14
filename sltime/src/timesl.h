/*
 * timesl.h
 *
 *  Created on: Sep 25, 2015
 *      Author: wocson
 */

#ifndef TIMESL_H_
#define TIMESL_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <ratio>
#include <chrono>

struct tm_sl {
	int	tm_sec;		/* seconds after the minute [0-60] */
	int	tm_min;		/* minutes after the hour [0-59] */
	int	tm_hour;	/* hours since midnight [0-23] */
	int	tm_mday;	/* day of the month [1-31] */
	int	tm_mon;		/* months since January [0-11] */
	int	tm_year;	/* years since 1900 */
	int	tm_wday;	/* days since Sunday [0-6] */
	int	tm_yday;	/* days since January 1 [0-365] */
	int	tm_isdst;	/* Daylight Savings Time flag */
	long	tm_gmtoff;	/* offset from UTC in seconds */
	char	*tm_zone;	/* timezone abbreviation */
};


class SlDate
{
public:

	friend std::ostream& operator<<(std::ostream& out, SlDate& o);

	SlDate(void);

private:
	int leto;
	int mesiac;
	int den;

	int chas;
	int chast;
	int dolia;

	struct tm * ti;
	struct tm_sl ts;
	struct timespec tms;

	struct tm_sl *sl_gmtime(const time_t *timer);
	struct tm_sl *sl_localtime(const time_t *timer);
	time_t sl_mktime(void);

};


#endif /* TIMESL_H_ */
