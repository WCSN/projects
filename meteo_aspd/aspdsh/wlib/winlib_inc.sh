#!/bin/bash
################################################################
##
## Windows Lib
##
## wocson (c)
##
## Requare: 
##

MBGRCOLOR=$BGBLUE
MFGRCOLOR=$GRAY
FGTITCOLOR=$BLACK
BGTITCOLOR=$BGGRAY

MMSSYM=$RED
MMLSYM=$BLACK
KEY=''
RETVAL=""

#eclare -A WINDOW		
declare -A Window_X		# Pos X
declare -A Window_Y		# Pos Y
declare -A Window_W		# Win W
declare -A Window_H		# Win H
declare -A Window_Name	# Name
declare -A LnCount
declare -A Window_FG
declare -A Window_BG

declare -A WinTxtBuffer
declare -A wintxtbuff_W
declare -A wintxtbuff_H

## $1-Номер окна
function getarsize()
{
  arsize=0
  if [ -z $1 ]; then I=0; N=$WinCount;
  else I=$1; N=$1; fi

  for (( i=I; i<=N; i++ )); do
	for (( j=0;  ; j++ )); do
	  if [ -z "${WinTxtBuffer[$i,$j]}" ]; then break; fi
	done
	let arsize+=$j
  done
}

## $1-Номер окна, $2-X, $3-Y, $4-text
function winouttext()
{
  local format; local txt;
  setcolor ${Window_FG[$1]}
  setbackcolor ${Window_BG[$1]}
  lentxt=`echo $4 | wc -m`
  format="%-${lentxt}s"
  txt=`printf "$format" "$4"`

  getarsize $1
  let LnCount[$1]++

  WinTxtBuffer[$1,${LnCount[$1]}]="$txt"
  outtextxy $((${Window_X[$1]}+$2)) $((${Window_Y[$1]}+$3)) "$txt"
}

## $1-NameAcc, $2-X, $3-Y, $4-W, $5-H, $6-Name, $7-fg, $8-bg
function setWinatr()
{
  Window_X["$1"]=$2
  Window_Y["$1"]=$3
  Window_W["$1"]=$4
  Window_H["$1"]=$5
  Window_Name["$1"]=$6

  Window_FG["$1"]=$7
  Window_BG["$1"]=$8

  WinTxtBuffer["$1",0]=""
  WinTxtBuffer_W["$1",0]=$4
  WinTxtBuffer_W["$1",0]=$5
}

## $1-NameAcc, $2-fg color, $3-bg color
function setcolorWindow()
{
  setcolor "$2"
  setbackcolor "$3"
  Window_FG["$1"]="$2"
  Window_BG["$1"]="$3"
}

## $1-NameAcc, $2-X, $3-Y, $4-W, $5-H, $6-fg, $7-bg
function Window()
{
  local x; local y; local LINE;

  setWinatr "$1" $2 $3 $4 $5 $6 $7
  setcolorWindow "$1" $6 $7

  Tln=`echo " $1 " | wc -m` # Title len

  for (( x=$(($2+1)); x<$(($2+$4)); x++ )); do LINE+="─"; done;

  local FCLNTOP="┌"; FCLNTOP+=$LINE; FCLNTOP+="┐";
  local FCLNBOD="│"; for (( x=$(($2+1)); x<$(($2+$4)); x++ )); do FCLNBOD+=" "; done; FCLNBOD+="│";
  local FCLNDWN="└"; FCLNDWN+=$LINE; FCLNDWN+="┘"

  outtextxy $2 $3 "$FCLNTOP"
  #Title
  outtextxy $(($2+2)) $3 " $1 "
  for (( y=$(($3+1)); y<$(($3+$5)); y++ )); do outtextxy $2 $y "$FCLNBOD"; done
  outtextxy $2 $y "$FCLNDWN\n"

  setcolor $FGTITCOLOR; setbackcolor $BGTITCOLOR
}

function Menu()
{
  local x; local LINE;

  local format="$MMSSYM$BGTITCOLOR%-${TC}s";
  printf "$format" " "

  format="$MMSSYM$BGTITCOLOR%-1s$MMLSYM$BGTITCOLOR%-s ";
  LINE="≡ "
  for pm in $1; do
	MS=`echo $pm | cut -d")" -f1`
	LS=`echo $pm | cut -d")" -f2-`
	LINE+=`printf "$format" "$MS" "$LS"`
  done
  outtextxy 1 1 "$LINE\n"
}

function KeyTab()
{
  KeyT=""
  kW=$(( ($TC-20)/10 ))
  format="$GRAY$BGBLACK%-2s$BLACK$BGCYAN%-${kW}s"
  KeyT+=`printf "$format" "F1" "Help "`
  KeyT+=`printf "$format" "F2" "Menu "`
  KeyT+=`printf "$format" "F3" "View "`
  KeyT+=`printf "$format" "F4" "Edit "`
  KeyT+=`printf "$format" "F5" "Copy "`
  KeyT+=`printf "$format" "F6" "Move "`
  KeyT+=`printf "$format" "F7" "Stat "`
  KeyT+=`printf "$format" "F8" "AdrBk"`
  KeyT+=`printf "$format" "F9" "Setup"`
  KeyT+=`printf "$format" "F10" "Exit "`
  outtextxy 1 $TL "$KeyT"
}

function StatusLine()
{
  Window "Status" 1 $(($TL-3)) $(($TC-1)) 2 $GRAY $BGBLUE	
}

function WindowClose()
{
  winouttext "$1" 1 1 "Close"
}

function getkeyp()
{
  KEY=''
  IFS= read -s -n 4 SELECT

  for (( i=0; i < ${#SELECT}; i++ )); do
	KEY+="$SELECT ";
  done
}

function WindowDialog()
{
  Window "Error" 33 15 66 20 $LYELLOW $BGRED
  winouttext "Error" 1 1 "Текст сообщения строка 1"
  winouttext "Error" 1 2 "Текст сообщения строка 2"
  winouttext "Error" 1 3 "Текст сообщения строка 3"

  setcolor $LYELLOW
  setbackcolor $BGGRAY
  winouttext "Error" 30 18 "YES"; winouttext "Error" 36 18 "NO";

  while true; do

  getkeyp

	case $KEY in
	y|Y) ret=y; break;
	;;
	n|N) ret=n; break;
	;;
	*)
	;;
	esac
  done
}

## $1-$WinCount
function FPanelUpdate()
{
  local y; local txt=""; local format; local lsz=11;
  szN=$((${Window_W[$1]}-30))

  #file list
  format="$GRAY$BGBLUE%-${szN}s$GRAY$BGBLUE│$RED$BGBLUE%11s$GRAY$BGBLUE│$GRAY$BGBLUE%15s"
  szPan=$((${Window_H[$1]}-2))
  txt=""

  for (( x=0; x<$szN; x++ )); do txt+="─"; done;

  format2="├$GRAY$BGBLUE%-${szN}s$GRAY$BGBLUE┴$GRAY$BGBLUE%-11s$GRAY$BGBLUE┼$GRAY$BGBLUE%-15s"
  txt=`echo -en "printf \"$format2\" \"$txt\" \"───────────\" \"────────────────┤\"" | zsh`
  winouttext $1 0 $szPan "$txt"
  winouttext $1 $(($x-2)) $szPan "┬";

  y=3; SUMMSIZE=0;
  txt=""
  while read LN ; do

	LN=`echo "$LN" | enca -x UTF-8`
  
	FNREAL=`echo "$LN" | awk '{for (i=9;i<=NF;i++) printf "%s ",$i}'`
    FNREAL=`echo "$FNREAL" | sed 's/[[:space:]]*$//'`

	glen=`echo "$FNREAL" | wc -m`
	hl=$(($szN/2-2))

	if (( $glen>=${Window_W[$1]} )); then
	  FS=`echo "$FNREAL" | cut -c1-$hl`
	  FE=`echo "$FNREAL" | rev | cut -c1-$hl | rev`
	  FNAME="$FS<=>$FE"
	else
	  FNAME=$FNREAL
	fi

	FSZBYTE=`echo "$LN" | awk '{ print $5 }'`
	glen=`echo "$FSZBYTE" | wc -m`

	if (( $lsz<=$glen )); then
  	  FSIZE="`echo \"scale=2; $FSZBYTE/1024/1024\" | bc`M"
	else
	  FSIZE="$FSZBYTE" 
	fi

	FDATE=`echo "$LN" | awk '{for (i=6;i<=8;i++) printf "%s ",$i}'`
	FDATE=`echo "$FDATE" | sed 's/[[:space:]]*$//'`	

	let SUMMSIZE+=FSZBYTE

	txt=`echo -en "printf \"$format\" \"$FNAME\" \"$FSIZE\" \"$FDATE\"" | zsh`

	if (( $y>=$szPan )); then break; fi
	winouttext $1 1 $y "$txt"
	let y++
  done <<< `ls -l $2/`
  cntf=$y

  for (( y=$y; y<$szPan; y++ )); do
	txt=`echo -en "printf \"$format\" \"\" \"\" \"\"" | zsh`
	winouttext $1 1 $y "$txt"
  done

  txt=""; let y-=4;
  szF=$(($szN-13))
  for (( x=0; x<$szF; x++ )); do txt+=" "; done;
  format3="$GRAY$BGBLUE%7s %-${szF}s$GRAY$BGBLUE│∑$GRAY$BGBLUE%13s$GRAY$BGBLUE│$GRAY$BGBLUE%15s"
  dt=`date "+%d %b %Y"`
  txt=`echo -en "printf \"$format3\" \"Файлов:\" \"$cntf$txt\" \"$SUMMSIZE\" \"$dt\"" | zsh`
  winouttext $1 1 $(($szPan+1)) "$txt";
}

# $1-NameAcc, $2-directory view
function FPanel()
{
  local i; local txt; local format;
  szN=$((${Window_W[$1]}-30))

  winouttext "$1" $(($szN+1)) 0 "┬"; winouttext "$1" $(($szN+13)) 0 "┬";

  format="$LYELLOW%-${szN}s$GRAY$BGBLUE│$LYELLOW$BGBLUE%-11s$GRAY$BGBLUE│$LYELLOW$BGBLUE%-15s"
  txt=`echo -en "printf \"$format\" \" Имя\" \" Размер\" \" Дата\"" | zsh`
  winouttext "$1" 1 1 "$txt"

  txt=""
  for (( x=0; x<$szN; x++ )); do txt+="─"; done;
  format="├$GRAY$BGBLUE%-${szN}s$GRAY$BGBLUE┼$GRAY$BGBLUE%-11s$GRAY$BGBLUE┼$GRAY$BGBLUE%-15s"
  txt=`echo -en "printf \"$format\" \"$txt\" \"───────────\" \"────────────────┤\"" | zsh`
  winouttext "$1" 0 2 "$txt"
  FPanelUpdate "$1" $2
  winouttext "$1" $(($szN-2)) ${Window_H["$1"]} "┴"; winouttext "$1" $(($szN+13)) ${Window_H["$1"]} "┴";
}

function CLOSE()
{
  clrscr
}
