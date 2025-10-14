##
## for getmeteo
##

WHO=$0

LOGDIR=/var/meteo/log
LOGFILE=meteo.log
LOGG=$LOGDIR/$LOGFILE

TIMER00=0; TIMERVAL=""; TIMER=OFF;  # ON OFF

################################################################
##
## Timer
##
## void timeroff void
##
function timeroff()
{
  TIMER=OFF
}

################################################################
##
## void timer void
##
## Get time from 0 any point
##
function timer()
{
  if [ $TIMER == ON ]; then
	TIMER00=$((`date +%s`-TIMER00))
	  if (( $TIMER00 >= 2764800 )); then
		TIMERVAL="$LRED \bНереально дохера времени"
	elif (( $TIMER00 >= 86400 )); then
		TIMERVAL=`date -u -v-1d -r $TIMER00 "+%dд %Hч %Mм %Sс"`
	elif (( $TIMER00 >= 3600 )); then
		TIMERVAL=`date -u -r $TIMER00 "+%Hч %Mм %Sс"`
	elif (( $TIMER00 >= 60 )); then
		TIMERVAL=`date -u -r $TIMER00 "+%Mм %Sс"`
	else
		TIMERVAL=`date -u -r $TIMER00 "+%Sс"`
	fi
	TIMER=OFF
  else
	let TIMER00=`date -u +%s`
	TIMER=ON
  fi
}

function isleap()
{
    year=$1
#    echo $year

   (( !(year % 4) && ( year % 100 || !(year % 400) ) )) && return 1 || return 0;
}


function logg()
{

 LOGOUT="`date -u '+%Y%m%d %H:%M:%S'` $WHO $1$2$3$4$5$6"

# echo "$LOGOUT"
# echo "LOG: $LOG"

 if [ ! -d $LOGDIR ]
    then mkdir -p $LOGDIR;
 fi
  echo $LOGOUT >> $LOGG
}

function lastday()
{
  dm=`date +%d%m`

if [ $dm == "3101" ] || [ $dm == "2802" ] || [ $dm == "2902" ] || [ $dm == "3103" ] \
|| [ $dm == "3004" ] || [ $dm == "3105" ] || [ $dm == "3006" ] || [ $dm == "3107" ] \
|| [ $dm == "3108" ] || [ $dm == "3009" ] || [ $dm == "3110" ] || [ $dm == "3011" ] \
|| [ $dm == "3112" ]; then
	echo "lastday"

else
	echo "ordinaryday"
fi
}

function message()
{
  echo $1 $2 $3
}

GENDALF=bell62RAE2017
KEEPCLEAN=/usr/bin/aruba
