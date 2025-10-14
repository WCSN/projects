'''
Created on 18 нояб. 2019 г.

@author: wocson
'''

from datetime import timedelta, datetime
from aspd_lib.config import conf

## TIME
# date = datetime.datetime.now()
# days_in_month = calendar.monthrange(date.year, date.month)[1]
# date -= timedelta(days = days_in_month)

def getmonth(shift='0'):
    month=getdatetime('%B', shift)
    month=month.replace('ря', 'рь');    month=month.replace('ля', 'ль');    month=month.replace('та', 'т')
    month=month.replace('ая', 'ай');    month=month.replace('ня', 'нь');    month=month.upper()
    return month

def timeshift(datenow, tmshift='0'):    # tmshift = '-1d', '+5h', '-7d'
    left=1; uptm=1; val=sm=''
    # lst = list(tmshift)
    for sm in tmshift:
        if sm=='-': left=-1
        elif sm=='h': uptm=60
        elif sm=='d': uptm=24*60
        elif sm=='m': uptm=27*24*60     # 27 days is BAD but waths make...
        elif sm.isdigit(): val+=sm

    val=int(val)*left*uptm
    return datenow+timedelta(minutes=val)

TIMER00=0; TIMER=False;  # True | False

def sec2htime(value=0):
    days=divmod(value, 86400)
    # days[0] = days and days[1] = seconds remaining after those days
    hours=divmod(days[1], 3600)
    minutes=divmod(hours[1], 60)
    LRED='\033[0;91m'         # Red

    if value>=2764800:
        TIMERVAL=LRED+'Нереально дохера'
    elif value>=86400:
        TIMERVAL="%iд %iч %iм %iс"%(days[0], hours[0], minutes[0], minutes[1])
    elif value>=3600:
        TIMERVAL="%iч %iм %iс"%(hours[0], minutes[0], minutes[1])
    elif value>=60:
        TIMERVAL="%iм %iс"%(minutes[0], minutes[1])
    else:
        TIMERVAL="%iс"%(minutes[1])
    return TIMERVAL

def getdatetime(fmttime=conf.getvalue('DTSTAMP'), tmshift='0'):
    ''' get date time and shifttime'''
    date=datetime.now()
    if tmshift!='0': date=timeshift(date, tmshift)
    tm="{:{fmt}}".format(date, fmt=fmttime)
    return tm

def getdatetimeUTC(fmttime=conf.getvalue('DTSTAMP'), tmshift='0'):
    ''' create directiries and set permissions '''
    date=datetime.utcnow()
    if tmshift!='0': date=timeshift(date, tmshift)
    tm="{:{fmt}}".format(date, fmt=fmttime)
    return tm

def timeroff():
    ''' Timer '''
    global TIMER; TIMER=False

def timer():
    ''' void timer void '''
    TIMERVAL='0c'
    global TIMER00, TIMER

    if TIMER:
        DELTAT=int(getdatetime('%s'))-TIMER00
        TIMERVAL=sec2htime(DELTAT)
        TIMER=False
    else:
        TIMER00=int(getdatetime('%s'))
        TIMER=True

    return TIMERVAL

timer()
