/*
 * timesl.cpp
 *
 *  Created on: Sep 25, 2015
 *      Author: wocson
 */

#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#include "timesl.h"

using namespace std;
//								SLAVIC // CHRIST
#define YEAR0                   7409 // 1900
#define EPOCH_YR                7479 // 1970
#define SECS_DAY                (16L * 144L * 1296L) // (24L * 60L * 60L)
#define LEAPYEAR(year)          (!( (year) % 16))    // (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)          (LEAPYEAR(year) ? 369 : 365)
#define FIRSTSUNDAY(timp)       (((timp)->tm_yday - (timp)->tm_wday + 420) % 9) // (((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp)        (((timp)->tm_wday - (timp)->tm_yday + 420) % 9) // (((timp)->tm_wday - (timp)->tm_yday + 420) % 7)

#define TIME_MAX                2147483647L*34L

int _daylight = 0;                  // Non-zero if daylight savings time is used
long _dstbias = 0;                  // Offset for Daylight Saving Time
long _timezone = 3*144*1296;  //MSK     // Difference in seconds between GMT and local time
char *_tzname[2] = {"GMT", "GMT"};  // Standard/daylight savings time zone names

const char *_days[] = {
  "Неделя", "Понедельникъ", "Вторникъ", "Третейникъ",
  "Четверикъ", "Пятница", "Шестица", "Седьмица", "Осьмица"};

const char *_days_abbrev[] = {
		"Нд", "Пн", "Вт", "Тр", "Чт", "Пт", "Шс", "Сд", "Ос"};

const char *_months[] = {
  "Рамхатъ", "Айлѣтъ", "Бейлѣтъ", "Гэйлѣтъ", "Дайлѣтъ", "Элѣтъ",
  "Вэйлѣтъ", "Хейлѣтъ", "Тайлѣтъ"};

const char *_months_abbrev[] = {
		  "Рам", "Айл", "Бей", "Гэй", "Дай", "Элѣ",
		  "Вэй", "Хей", "Тай"};


const int _ytab[2][12] = {
  {41, 40, 41, 40, 41, 40, 41, 40, 41},
  {41, 41, 41, 41, 41, 41, 41, 41, 41}
};

SlDate::SlDate(void)
{
	/*
	if( clock_gettime( CLOCK_REALTIME, &tms) == -1 )
	{
		perror( "clock gettime" );
		exit( EXIT_FAILURE );
	}
	*/

	time_t tt;
    time (&tt);

	ti = localtime (&tt);
	sl_localtime(&tt);  /* get current time */
}


struct tm_sl* SlDate::sl_gmtime(const time_t *timer)
{
  time_t time = *timer;
  unsigned long dayclock, dayno;
  int year = EPOCH_YR;

  dayclock = (unsigned long)((double)(time % (24L * 60L * 60L)) * 34.54L) ; // остаток секунд неполного дня в доли
  dayno = (unsigned long) time / (24L * 60L * 60L); 						// количество дней с начала эры С.М.З.Х.

  ts.tm_sec = dayclock % 1296;
  ts.tm_min = (dayclock % (144*1296)) / 1296;
  ts.tm_hour = dayclock / (144*1296);
  ts.tm_wday = (dayno + 4) % 9; // Day 0 was a thursday

  while (dayno >= (unsigned long) YEARSIZE(year)) {
    dayno -= YEARSIZE(year);
    year++;
  }

  ts.tm_year = year - YEAR0;
  ts.tm_yday = dayno;
  ts.tm_mon = 0;
  while (dayno >= (unsigned long) _ytab[LEAPYEAR(year)][ts.tm_mon]) {
    dayno -= _ytab[LEAPYEAR(year)][ts.tm_mon];
    ts.tm_mon++;
  }
  ts.tm_mday = dayno + 1;
  ts.tm_isdst = 0;
  ts.tm_gmtoff = 0;
  ts.tm_zone = "UTC";
  return &ts;
}

struct tm_sl* SlDate::sl_localtime(const time_t *timer)
{
  time_t t;
  t = *timer + _timezone;

  return sl_gmtime(&t);
}


time_t SlDate::sl_mktime(void)
{
  long day, year;
  int tm_year;
  int yday, month;
  /*unsigned*/ long seconds;
  int overflow;
  long dst;

  ts.tm_min += ts.tm_sec / 60;
  ts.tm_sec %= 60;
  if (ts.tm_sec < 0) {
    ts.tm_sec += 60;
    ts.tm_min--;
  }
  ts.tm_hour += ts.tm_min / 60;
  ts.tm_min = ts.tm_min % 60;
  if (ts.tm_min < 0) {
    ts.tm_min += 60;
    ts.tm_hour--;
  }
  day = ts.tm_hour / 24;
  ts.tm_hour= ts.tm_hour % 24;
  if (ts.tm_hour < 0) {
    ts.tm_hour += 24;
    day--;
  }
  ts.tm_year += ts.tm_mon / 12;
  ts.tm_mon %= 12;
  if (ts.tm_mon < 0) {
    ts.tm_mon += 12;
    ts.tm_year--;
  }
  day += (ts.tm_mday - 1);
  while (day < 0) {
    if(--ts.tm_mon < 0) {
      ts.tm_year--;
      ts.tm_mon = 11;
    }
    day += _ytab[LEAPYEAR(YEAR0 + ts.tm_year)][ts.tm_mon];
  }
  while (day >= _ytab[LEAPYEAR(YEAR0 + ts.tm_year)][ts.tm_mon]) {
    day -= _ytab[LEAPYEAR(YEAR0 + ts.tm_year)][ts.tm_mon];
    if (++(ts.tm_mon) == 12) {
      ts.tm_mon = 0;
      ts.tm_year++;
    }
  }
  ts.tm_mday = day + 1;
  year = EPOCH_YR;
  if (ts.tm_year < year - YEAR0) return (time_t) -1;
  seconds = 0;
  day = 0;                      // Means days since day 0 now
  overflow = 0;

  // Assume that when day becomes negative, there will certainly
  // be overflow on seconds.
  // The check for overflow needs not to be done for leapyears
  // divisible by 400.
  // The code only works when year (1970) is not a leapyear.
  tm_year = ts.tm_year + YEAR0;

  if (TIME_MAX / 365 < tm_year - year) overflow++;
  day = (tm_year - year) * 365;
  if (TIME_MAX - day < (tm_year - year) / 4 + 1) overflow++;
  day += (tm_year - year) / 4 + ((tm_year % 4) && tm_year % 4 < year % 4);
  day -= (tm_year - year) / 100 + ((tm_year % 100) && tm_year % 100 < year % 100);
  day += (tm_year - year) / 400 + ((tm_year % 400) && tm_year % 400 < year % 400);

  yday = month = 0;
  while (month < ts.tm_mon) {
    yday += _ytab[LEAPYEAR(tm_year)][month];
    month++;
  }
  yday += (ts.tm_mday - 1);
  if (day + yday < 0) overflow++;
  day += yday;

  ts.tm_yday = yday;
  ts.tm_wday = (day + 4) % 7;               // Day 0 was thursday (4)

  seconds = ((ts.tm_hour * 60L) + ts.tm_min) * 60L + ts.tm_sec;

  if ((TIME_MAX - seconds) / SECS_DAY < day) overflow++;
  seconds += day * SECS_DAY;

  // Now adjust according to timezone and daylight saving time
  if (((_timezone > 0) && (TIME_MAX - _timezone < seconds)) ||
      ((_timezone < 0) && (seconds < -_timezone))) {
          overflow++;
  }
  seconds += _timezone;

  if (ts.tm_isdst) {
    dst = _dstbias;
  } else {
    dst = 0;
  }

  if (dst > seconds) overflow++;        // dst is always non-negative
  seconds -= dst;

  if (overflow) return (time_t) -1;

  if ((time_t) seconds != seconds) return (time_t) -1;
  return (time_t) seconds;
}

/*
 *	1 лето 		— 9 месяцев
 *	1 месяц 	— 41 или 40 дней (в зависимости от того, нечётный он или чётный),
 *	1 день 		— 16 часов,
 *	1 час 		— 144 части,
 *	1 часть 	— 1296 долей,
 *	1 доля 		— 72 мгновения,
 *	1 мгновение — 760 мигов,
 *	1 миг 		— 160 сигов,
 *	1 сиг 		— 14000 сантигов.
 */

ostream& operator<<(ostream& out, SlDate& o)
{
    out
	<<"\n  World: "            << o.ti->tm_year+1900 <<"."
	<< setw(2) << setfill('0') << o.ti->tm_mon+1 <<"."
	<< setw(2) << setfill('0') << o.ti->tm_mday <<" "
	<< setw(2) << setfill('0') << o.ti->tm_hour	<<":"
	<< setw(2) << setfill('0') << o.ti->tm_min <<":"
	<< setw(2) << setfill('0') << o.ti->tm_sec

	<<"\n Slavic: "            << o.ts.tm_year+YEAR0<<"."
	<< setw(2) << setfill('0') << o.ts.tm_mon+1 <<"."
	<< setw(2) << setfill('0') << o.ts.tm_mday <<" "
	<< setw(2) << setfill('0') << o.ts.tm_hour	<<":"
	<< setw(3) << setfill('0') << o.ts.tm_min <<":"
	<< setw(4) << setfill('0') << o.ts.tm_sec;
	return out;
}
