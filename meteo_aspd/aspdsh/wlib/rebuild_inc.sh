#########################################################
##
## Include file for reduild: rebuild_all, rebuild_day
##
## wocson (c)
##

SRC="/mnt/Vostok/mawslogs/`date +%Y`/log`date +%m`"
DST="/var/meteo/rebuild/dst"
METEOFILESDIR=/mnt/Vostok/meteo

DTFMT="+%d.%m %H:%M:%S"		# man date
WHO=`echo $0 | rev | cut -d "/" -f1 | rev`
WORKTYPE=work # work debug
LOGDIR=/var/meteo/log
LOG=$LOGDIR/rebuild.log

FNAME=""
inc=""
dstf=""

################################################################
##
## logging function
##
function logg()
{
  if ! [ -d $LOGDIR ]; then mkdir -p `echo $LOG | rev | cut -d"/" -f2- | rev`; fi
  TEXTOUT="`date \"$DTFMT\"` [$WHO] "
  for arg in "$@"; do if ! [ "$arg" == "stdout" ]; then TEXTOUT+="$arg"; fi; done
  echo -en "$TEXTOUT\n" >> $LOG
  if [ "$1" == "stdout" ] || [ $WORKTYPE == debug ]; then echo -en "$TEXTOUT\n"; fi
  chmod 666 $LOG
}

#########################################################
##
##	SetProgressBar $countMax "Name:"
##
function SetProgressBar()
{
  inc=$(( $1/100 ))
  printf "$2\33[37C"
}

#########################################################
##
##	ProgressBar $count
##
function ProgressBar()
{
  var="$1"; let var++;
  var=$(( $var/$inc ))
  if (( $var>100 ));then var=100; fi
  progress=$(( $var*30/100 ))
  bar=`printf "\33[47m% ${progress}s\33[m" ""`
  printf "\33[37D %-38s  %-4s" "${bar}" "$var%"
}

function rebuild()
{
if ! [ -z "$1" ]; then
  let TIMER=`date +%s`
  local buffer=()
  sfname=`echo "$1" | rev | cut -f1 -d"/" | rev`

  buffer=`cat "$1" | grep "DATE"`
  countMax=`echo "$buffer" | wc -l | cut -f5 -d" "`

  dfname=${sfname/l0`date +%y`/`date +%Y`}
  ffname=$DST/${dfname/.csv/.log}

  if ! [ -z "$3" ]; then dfname=$3; ffname="./$dfname"; fi

  FNAME=$ffname
  logg "rebuild $sfname to $dfname"
  logg "save to $ffname"

  if ! [ $2 == OFF ]; then SetProgressBar $countMax "$sfname >> $dfname:"; fi
  local count=0

  echo -en "" > $ffname
  while IFS= read -r line; do
    tmp="`echo \"$line\" | cut -f2-`"
    echo -e "List\t$tmp" >> $ffname
  	if ! [ $2 == OFF ]; then ProgressBar $(( count++ )); fi
  done <<< "$buffer"
  let TIMER=`date +%s`-TIMER
  c=s;
  if ! [ $2 == OFF ]; then echo " $TIMER$c"; fi
fi
}
