#################################################################
## ASPD changer
## aspd_inc.sh  
##
## wocson (c) 2020
##
#################################################################
VERSION=1.09S
# Status: Code close for new functions. Set version 1.005 
# Last change: remove if [ -z "${strlet##$"ATTACH:"*}" ];parceletter_vrs, parceletter_vos_svc -> parceletter, add conv \n -> \r\n
# Code open only error correct.
# Next version will be only python3
#
Requaired="sudo mc mcedit mcview rar 7z ncat awk(gawk) sed zsh lftp enca mutt"
##
source $LIB/sysfns_inc.sh
source $LIB/tcolor_inc.sh

LOG="$ASPDLOGDIR/aspd.log"
WSLZ=$WIDESCREEN	# Wide Screen set in aspd.conf

DTFMT="+%d.%m %H:%M:%S"		# see man date - date different Linux and BSD - this use BSD'variant

# Special dirs
MANAGE=$INFODIR/manage
SCANS=$INFODIR/scans
MATTACHDIR=$INFODIR/Mattach

#LOGinfo files
FLLSLOG=$ASPDLOGDIR/fsls.log
FTPSLOG=$ASPDLOGDIR/ftps.log
FTPRLOG=$ASPDLOGDIR/ftpr.log
FTPLLOG=$ASPDLOGDIR/ftpl.log
MAILLOG=$ASPDLOGDIR/mail.log
STASAVE=$ASPDLOGDIR/stat.sav

# Archive directries
LDIR_ARCH=$ASPDWORKDIR/L_Archive/RAE$NUMEXP/`date +%Y-%m` # local date
RDIR_ARCH=$ASPDWORKDIR/R_Archive/RAE$NUMEXP/`date +%Y-%m` # local date
ARCHDIR=`date +%m%d` # local date

# GLOBAL variables
COUNTLINES=0; COUNTADRESS=0; COUNTPATHS=0; CONTROL=1; CHEEFLINE="OFF"; INSERTLIST="OFF";
CNVUTF8=""; TXTFNM=""; TXTFSZ=""; FSZBYTE=0; FLDATE=""; FNREAL=""; FLMODE=""; SUMSIZE=0; 
NXTIMESIN="0"; CRTIMESIN="0"; TIMESIN=""; countcmd=0; szbefore=0; nMAILSND=0; nFTPSND=0; 
ErrLN=""; RETCHS=0; LDTTM=""; nFLSND=0; nGRP=-1; XLENA=0; XPATH=0; XCMNT=0;
SIGN=""; BIGSIZE=OFF; TYPEVIEW="FLS"; NUMSESSNDOFDAY=1; TYPEMIX="OFF";

SINTMIN="`echo $SIN0 | cut -d'-' -f1 | cut -d':' -f2`"
SINTMAX="`echo $SIN0 | cut -d'-' -f2 | cut -d':' -f2 | cut -d' ' -f1`"

declare -a aFLS aLSZ				# список файлов	| список размеров файлов
declare -A WABLNS	WADRGROUP		# lines from abook | groups adresses
declare -a WNMGROUP	WADRESS WFTPPTH	# names group |  adresses | ftp paths	

declare -a WMEMAILAD	# e-mail adresses 	tag: e-mail: | E-MAIL:
declare -a WMSUBJECT	# e-mail subjects 	tag: SUBJ:
declare -a WMTXTMAIL	# e-mail text		tag: TMAIL:
declare -a WMATTACHM	# e-mail attach		set from letter

declare -a WLETTXT		# letter text
declare -a WSUBJCT		# letter subject
declare -A WLFNPTH		#
declare -A WRFNPTH		#
declare -a PTFATCH		# attachment for vos/svc tag: attach:|ATTACH:
declare -a NMFATCH		# attachment for vos/svc tag: attach:|ATTACH:

# Main constants colors
MBGRCOLOR=$BGBLUE	# Main background color
MFGRCOLOR=$GRAY		# Main foreground (text)color
MFLBCOLOR=$GRAY		# Main files bb color
MPTHCOLOR=$LCYAN	# Main paths color
MSZVCOLOR=$RED		# Main Size Files Billborard
MDTFCOLOR=$WHITE	# Main Date Color Billborard
MTBTCOLOR=$BLACK	# Main Title BillBoard Text
MTBBCOLOR=$BGGRAY	# Main Title BillBoard Background
MTBNCOLOR=$LYELLOW	# Main Title Column BillBoardList Name
MTBDCOLOR=$WHITE	# Main BillBoardList DIR
MTETCOLOR=$LRED		# Error Text
MTEBCOLOR=$BGBLUE	# Error Background

MBGSCOLOR=$BGBLUE	# Background color Select
MFLSCOLOR=$BIWHITE	# Foreground color Select
MSZSCOLOR=$BRED		# Size color Select

# Work common colors for screen outs
TBCOLOR=$MFGRCOLOR	# Text/Line  Billborard
BBCOLOR=$MBGRCOLOR	# BackGround Billborard

FBCOLOR=$MFLBCOLOR	# Files Billborard
PBCOLOR=$MPTHCOLOR	# Path Billborard
SZCOLOR=$MSZVCOLOR	# Size Files Billborard
DTCOLOR=$MDTFCOLOR 	# Date Color Billborard

TBTCOLOR=$MTBTCOLOR	# Title BillBoard Text
TBBCOLOR=$MTBBCOLOR	# Title BillBoard Backgr

BNCOLOR=$MTBNCOLOR	# Title BillBoardList Name
BDCOLOR=$MTBDCOLOR	# BillBoardList DIR
ETCOLOR=$MTETCOLOR	# Error Text
EBCOLOR=$MTEBCOLOR	# Error Background

BGSCOLOR=$MBGSCOLOR	# BG BB Select
FLSCOLOR=$MFLSCOLOR	# File BB Select
SZSCOLOR=$MSZSCOLOR	# Size BB Select

#SIGNAL files
LOCKED_SEND="/tmp/lock_send_$WORKTYPE"
LOCKED_RECV="/tmp/lock_recv_$WORKTYPE"
LOCKED_MAIL="/tmp/lock_mail_$WORKTYPE"

SIGN_SEND="$INFODIR/send"
SIGN_DELAY="$INFODIR/delay"

#TMP files
TMP="/tmp/aspd.tmp"
TMPFILE="$TMPASPD/tmpfl.tmp"
TMPSTORE="$INFODIR/tmpstore"
LASTMT="$TMPASPD/lastmt.tmp"

## Index files for send/recv
if [ $WORKTYPE == debug ]; then DBTYPE="_debug"; fi

INDEX_AVS="$DBDIR/db$DBTYPE/index.avs"		# index avos
INDEX_CLM="$DBDIR/db$DBTYPE/index.clm"     # index clim
INDEX_SVC="$DBDIR/db$DBTYPE/index.svc"     # index svc
INDEX_VOS="$DBDIR/db$DBTYPE/index.vos"     # index vos
INDEX_VRS="$DBDIR/db$DBTYPE/index.vrs"     # index vrs
INDEX_SND="$DBDIR/db$DBTYPE/index.snd"     # index send
INDEX_RCV="$DBDIR/db$DBTYPE/index.rcv"     # index recv
INDEX_DEL="$DBDIR/db$DBTYPE/index.del"     # index del
INDEX_EML="$DBDIR/db$DBTYPE/index.eml"     # index email

permdr="u-r-w-x,g-r-w-x,o-r-w-x"
perm="u+r+w+X,g+r+w+X,o-r-w-x"

permAllrw="u+r+w-x,g+r+w-x,o+r+w-x"
permAllrwx="u+r+w+x,g+r+w+x,o+r+w+x"
permAllrwX="u+r+w+X,g+r+w+X,o+r+w+X"

permGOr="u+r+w-x,g+r-w-x,o+r-w-x"
permGOrx="u+r+w+x,g+r-w+x,o+r-w+x"
permGOrX="u+r+w+X,g+r-w+X,o+r-w+X"

permOr="u+r+w+x,g+r+w+x,o+r-w-x"
permOrx="u+r+w+x,g+r+w+x,o+r-w+x"
permOrX="u+r+w+X,g+r+w+X,o+r-w+X"

owner="root:aspd"

################################################################
##
## create directiries and set permissions
##
## void gettimedate void
##
function gettimedate()
{
  local fmttime="$1";
  if [ -z $fmttime ]; then fmttime="$DTSTAMP"; fi
  LDTTM=`date "+$fmttime"`	# local date
}

################################################################
##
## create directiries and set permissions
##
## void permInfodir void
##
function permInfodir()
{
# aspd files dir
  if ! [ -z $INFODIR ]; then
	sudo chmod -Rf "$perm"		 "$INFODIR" &> /dev/null
	sudo chmod -f  "$permOrX"	 "$INFODIR" &> /dev/null
	sudo chmod -Rf "$permAllrw"	 "$ASPDLOGDIR" &> /dev/null
	sudo chmod -f  "$permAllrwX" "$ASPDLOGDIR" &> /dev/null
	sudo chmod -Rf "$permOrx" 	 "$BINDIR" &> /dev/null
	sudo chmod -f  "$permOrX"	 "$BINDIR" &> /dev/null
	sudo chmod -Rf "$permAllrw"	 "$UNKNOWNDIR" &> /dev/null
	sudo chmod -f  "$permAllrwX" "$UNKNOWNDIR" &> /dev/null
	sudo chmod -Rf "$permOrx"  	 "$MANAGE" &> /dev/null
	sudo chmod -f  "$permOrX" 	 "$MANAGE" &> /dev/null

	sudo chmod -Rf "$permAllrw"	 "$MATTACHDIR" &> /dev/null
	sudo chmod -f  "$permAllrwX" "$MATTACHDIR" &> /dev/null

	sudo chmod -Rf "$perm" 		 "$SCANS" &> /dev/null
	sudo chmod -f  "$perm" 		 "$SCANS" &> /dev/null

	sudo chmod -f  "$permGOrX" 	 "$TEMPLATES" &> /dev/null
	sudo chmod -f  "$permGOr"  	 "$TEMPLATES"/* &> /dev/null

	sudo chmod -Rf "$permAllrw"	 "$TMPASPD" &> /dev/null
	sudo chmod -f  "$permAllrwX" "$TMPASPD" &> /dev/null

	sudo chown -Rf "$owner" 	 "$INFODIR" &> /dev/null

#aspd db dir

	sudo chmod -Rf "$perm" 		 "$DBDIR" &> /dev/null
	sudo chmod -f  "$perm" 		 "$DBDIR" &> /dev/null
	sudo chown -Rf "$owner" 	 "$DBDIR" &> /dev/null
  fi
}


#############################################################
##
## setallperm setting all permision for work directories
##
## void setallperm void
##
function setallperm()
{
  local opt=$1

  if [[ -z $opt || $opt == "-v" ]]; then
	opt="Rf${opt#"-"}"
	infoline "Установка прав (Opt: -$opt)"
	sudo chmod -Rf "$permdr"  "$ASPDWORKDIR" &> /dev/null
	outt "Каталог $ASPDWORKDIR"
	outt "Права: $perm / (f:660/d:770)"
	outt "Владелец: $owner . Ожидайте"
	sudo chmod -$opt "$perm"  "$ASPDWORKDIR" &> /dev/null
	sudo chown -$opt "$owner" "$ASPDWORKDIR" &> /dev/null
	permInfodir
	outt "Установка прав завершена"
  else
	settextcolor $LRED
	outt "Неверный параметр $opt для комманды setperm"
	help sp
	outt "Смотрите: man chown / man chmod"
	setdefcolors
  fi
  status
}

################################################################
##
## create files and set permissions
##
## void crfsetpm file
##
function crfsetpm()
{
  local FILE="$1"  PERM="$2";

  if [ -z "$FILE" ]; then return; fi
  if [ -z $PERM ]; then PERM="$perm"; fi

  if ! [ -f "$FILE" ]; then sudo touch "$FILE"; fi

  sudo chmod "$PERM"  "$FILE" &> /dev/null
  sudo chown "$owner" "$FILE" &> /dev/null
}

################################################################
##
## create directiries and set permissions
##
## void crdsetpm dir
##
function crdsetpm()
{
  local DIR="$1" PERM="$2";

  if [ -z "$DIR" ]; then return; fi
  if [ -z $PERM ]; then PERM=$perm; fi

  if ! [ -d "$DIR" ]; then sudo mkdir -p "$DIR"; fi

  sudo chmod "$PERM"  "$DIR" &> /dev/null
  sudo chown "$owner" "$DIR" &> /dev/null
}

## Create directories
if ! [ -d "$LDIRFORSEND" ]; then crdsetpm "$LDIRFORSEND"; 	fi
if ! [ -d "$LDIRRECV"    ]; then crdsetpm "$LDIRRECV";		fi
if ! [ -d "$LDIRSEND"    ]; then crdsetpm "$LDIRSEND";		fi
if ! [ -d "$LDIRDELAY"   ]; then crdsetpm "$LDIRDELAY";		fi

if ! [ -d "$INFODIR"     ]; then crdsetpm "$INFODIR" 	"$permOrX";		fi
if ! [ -d "$BINDIR"      ]; then crdsetpm "$BINDIR"   	"$permOrX"; 	fi
if ! [ -d "$ASPDLOGDIR"  ]; then crdsetpm "$ASPDLOGDIR"	"$permAllrwX";	fi
if ! [ -d "$SCANS"  	 ]; then crdsetpm "$SCANS"		"$perm";		fi
if ! [ -d "$UNKNOWNDIR"  ]; then crdsetpm "$UNKNOWNDIR"	"$permAllrwX";	fi
if ! [ -d "$TEMPLATES"   ]; then crdsetpm "$TEMPLATES"	"$permGOrX";	fi
if ! [ -d "$TMPASPD"     ]; then crdsetpm "$TMPASPD"   	"$permAllrwX"; 	fi
if ! [ -d "$TMPSTORE"    ]; then crdsetpm "$TMPSTORE"  	"$permAllrwX"; 	fi

if ! [ -d "$MATTACHDIR"  ]; then crdsetpm "$MATTACHDIR"	"$permAllrwX"; 	fi

if ! [ -f "$LOG" ]; then crfsetpm "$LOG" "$permAllrwX"; fi

############################################################
##
##	void setcolors FG BG FILE PATH
##
function setcolors()
{
  local TXTCL="$1" BGCL="$2" FGCL="$3" PTCL="$4";

  if [ -z $TXTCL ]; then TBCOLOR=$MFGRCOLOR;
  else TBCOLOR=$TXTCL; fi

  if [ -z $BGCL ]; then BBCOLOR=$MBGRCOLOR;
  else BBCOLOR=$BGCL; fi

  if [ -z $FGCL ]; then FBCOLOR=$MFLBCOLOR;
  else FBCOLOR=$FGCL; fi

  if [ -z $PTCL ]; then PBCOLOR=$MPTHCOLOR;
  else PBCOLOR=$PTCL; fi
}

############################################################
##
##	void setbackcolor BBCOLOR
##
function setbackcolor()
{
  local BBC="$1"
  if [ -z $BBC ]; then BBCOLOR=$MBGRCOLOR;
  else BBCOLOR=$BBC; fi
}

############################################################
##
##	void settextcolor TBCOLOR
##
function settextcolor()
{
  local TXC="$1"
  if [ -z $TXC ]; then TBCOLOR=$MFGRCOLOR;
  else TBCOLOR=$TXC; fi
}

############################################################
##
##	void settitlebackcolor BBCOLOR
##
function settitlebackcolor()
{
  local BBC="$1"
  if [ -z $BBC ]; then TBBCOLOR=$MTBBCOLOR;
  else TBBCOLOR=$BBC; fi
}

############################################################
##
##	void settextcolor TBCOLOR
##
function settitletextcolor()
{
  local TXTC="$1"
  if [ -z $TXTC ]; then TBTCOLOR=$MTBTCOLOR;
  else TBTCOLOR=$TXTC; fi
}

############################################################
##
##	void setpathcolor FBCOLOR
##
function setfilecolor()
{
  local FBC="$1"
  if [ -z $FBC ]; then FBCOLOR=$MFLBCOLOR;
  else FBCOLOR=$FBC; fi
}

############################################################
##
##	void setfilecolor PBCOLOR
##
function setpathcolor()
{
  local PBC="$1"
  if [ -z $PBC ]; then PBCOLOR=$MPTHCOLOR;
  else PBCOLOR=$PBC; fi
}

############################################################
##
##	void setpathcolor SZCOLOR
##
function setsizecolor()
{
  local SZC="$1"
  if [ -z $SZC ]; then SZCOLOR=$MSZVCOLOR;
  else SZCOLOR=$SZC; fi
}

############################################################
##
##	void setpathcolor DTCOLOR
##
function setdatecolor()
{
  local DTC="$1"
  if [ -z $DTC ]; then DTCOLOR=$MDTFCOLOR;
  else DTCOLOR=$DTC; fi
}

############################################################
##
##	void setdefcolors void
##
function setdefcolors()
{
  BBCOLOR=$MBGRCOLOR	# BackGround Billborard
  TBCOLOR=$MFGRCOLOR	# Text/Line  Billborard
  FBCOLOR=$MFLBCOLOR	# Files Billborard
  PBCOLOR=$MPTHCOLOR	# Path Billborard
  SZCOLOR=$MSZVCOLOR	# Size Files Billborard
  DTCOLOR=$MDTFCOLOR 	# Date Color Billborard
  TBTCOLOR=$MTBTCOLOR	# Title BillBoard text
  TBBCOLOR=$MTBBCOLOR	# Title BillBoard backgr
  BNCOLOR=$MTBNCOLOR	# Title BillBoardList Name
  BDCOLOR=$MTBDCOLOR	# BillBoardList DIR
  ETCOLOR=$MTETCOLOR	# Error Text
  EBCOLOR=$MTEBCOLOR	# Error Background

  BGSCOLOR=$MBGSCOLOR	# BackGround Select
  FLSCOLOR=$MFLSCOLOR	# File BB Select
  SZSCOLOR=$MSZSCOLOR	# Size BB Select
}

#############################################################
##
## setadrbook geterate internal a.book for send
##
## void setadrbook void
##
function getadrbook()
{
  local n=0 i=0 adrline="" tmp lnz
  while read -r -u5 adrline ; do

	if ! [ -z "${adrline##$'#'*}" ]; then if (( "${#adrline}" > 0 )); then
	  if  [ -z "${adrline##*$'['*}" ]; then
		let nGRP++
		WNMGROUP[$nGRP]=`echo -en "$adrline"|cut -c2-|rev|cut -c2-|rev`	# All name group
		WADRGROUP[${WNMGROUP[$nGRP]}]=
		WABLNS[$i]="@${WNMGROUP[$nGRP]}"
	  else
		WADRESS[$n]="${adrline%%';'*}"
		lnz=$((${#WADRESS[$n]}+1)); if (( $lnz > $XLENA )); then XLENA=$lnz; fi

		WADRGROUP[${WNMGROUP[$nGRP]}]+="${WADRESS[$n]};" 	# All adress in group

		tmp="${adrline:$lnz}"; 
		WFTPPTH[$n]="${tmp%%';'*}"
		lnz=$((${#WFTPPTH[$n]}+1)); if (( $lnz > $XPATH )); then XPATH=$lnz; fi

		tmp="${tmp:$lnz}"; tmp="${tmp//'  '/''}"; tmp="${tmp#' '}"

		if ! [ -z "${tmp##*$'#'*}" ]; then WCOMNTS[$n]="${tmp%%';'*}"
		else WCOMNTS[$n]=""; fi

		lnz=$((${#WFTPPTH[$n]}+1)); if (( $lnz > $XCMNT )); then XCMNT=$lnz; fi

		WABLNS[$i]="$adrline"
		let n++
	  fi
	  let i++; fi
	fi
  done 5< "$ADRBOOK"

  COUNTADRESS=$n
  COUNTGRADR=$i
}

#############################################################
##
## abook show a.book
##
## void abook void
##
function abook()
{
  local ot="$1"
  if [ -z $ot ]; then ot="-s"; fi

  getadrbook
  local i n=0 lz formatA fG0 fG1 FG2 lnz lnif ppz=3 adz=$(($XLENA-1)) ptz=$(($XPATH-1)) ngz topl TEXTFMT="";
  infoline -s "Адресная книга"
  lz=$(($WSLZ-(ppz+adz+ptz+5)))

  topl="┌";  topl+=`printf "─%.s" $(seq $ppz)`; topl+="┬"; topl+=`printf "─%.s" $(seq $adz)`; 
  topl+="┬"; topl+=`printf "─%.s" $(seq $ptz)`; topl+="┬"; topl+=`printf "─%.s" $(seq $lz)`; topl+="┐"; 
  outt "$ot" "$topl";

  PPCL=$LYELLOW$BBCOLOR;ADRCL="$LYELLOW$BBCOLOR"; PTCL="$LYELLOW$BBCOLOR"; CMTCL="$LYELLOW$BBCOLOR"; DEFCL=$GRAY$BBCOLOR
  formatA="│$PPCL%${ppz}s$DEFCL│$ADRCL%-${adz}s$DEFCL│$PTCL%-${ptz}s$DEFCL│$CMTCL%-${lz}s$DEFCL│"

  lnif=`echo -en "printf \"$formatA\" \"# \" \" АДРЕСCАТ\" \" КАТАЛОГ FTP/e-mail\" \" Примечание\"" | zsh`
  outt "$ot" "$lnif"

  ADRCL="$WHITE$BBCOLOR"; PTCL="$LCYAN$BBCOLOR"; CMTCL="$GRAY$BBCOLOR";  GRPCL="$BICYAN$BBCOLOR"; 
  formatA="│$PPCL%${ppz}s$DEFCL│$ADRCL%-${adz}s$DEFCL│$PTCL%-${ptz}s$DEFCL│$CMTCL%-${lz}s$DEFCL│"
  
  fG0="├`printf "─%.s" $(seq $ppz)`"
  fG2+="┼$GRAY$BBCOLOR%-${ptz}s$GRAY$BBCOLOR┼$GRAY$BBCOLOR%-${lz}s$GRAY$BBCOLOR┤"
  lnif=`printf "─%.s" $(seq $lz)`;

  for (( i=0; i<$COUNTGRADR; i++ )); do
	# group
	if [ -z "${WABLNS[$i]##$'@'*}" ]; then
	  # 1. ${str:1} - delete first symbol 2. ${str#"@"} - delete all @
	  ngz=${#WABLNS[$i]}
	  lnz=$((adz-ngz-1))
	  fG1="┤$GRPCL%-${ngz}s$GRAY$BBCOLOR%${lnz}s"; LNaG=`printf \"─%.s\" $(seq $lnz)`; LNfP=`printf \"─%.s\" $(seq $ptz)`; 
	  fmtG="$fG0$fG1$fG2"
	  TEXTFMT=`echo -en "printf \"$fmtG\" \" ${WABLNS[$i]:1}\" \" $LNaG\" \"$LNfP\" \"$lnif\"" | zsh`
	# adress
	else
	  comment="${WCOMNTS[$n]}"; comment="${comment:0:$lz}"
	  TEXTFMT=`echo -en "printf \"$formatA\" \"$(($n+1))\" \"${WADRESS[$n]}\" \"${WFTPPTH[$n]}\" \"$comment\"" | zsh`
	  let n++
	fi
	outt "$ot" "$TEXTFMT"
  done

  lnif="$topl"
  lnif=${lnif//'┌'/'└'}; lnif=${lnif//'┬'/'┴'}; lnif=${lnif//'┐'/'┘'}; 
  outt "$ot" "$lnif";
  status -s
}

#############################################################
##
## version out of scteen and other place version aspd
##
## void version void
##
function version()
{
  local TEXTVER=" asdp v$VERSION (c) wocson (bash script)"
  settextcolor $WHITE
  local format="$TBCOLOR$BBCOLOR%-${WSLZ}s$NORMAL$BGDEF\n"
  printf "$format" "$TEXTVER"
  setdefcolors
}

################################################################
##
## TRACE helper for debug
##
## void TRACE ["txt"] [ ... ]
##
function TRACE()
{
  local TEXTOUT
  if [ $WORKTYPE == work ]; then return 0; fi
  TEXTOUT="$LRED \bTRACE -----------------------------------------\n"
  TEXTOUT+="$LRED \bTRACE $GREEN"
  for arg in "$@"; do TEXTOUT+="$arg ";  done
  TEXTOUT+="\n$LRED \bTRACE -----------------------------------------\n"
  echo -en "$TEXTOUT$STDCL\n"
  set -x
}

function TRACEOFF()
{
  set +x
}

################################################################
##
## outt printing function
##
## void outt [-l|-s] "text"
##
function outt()
{
  local TXTOUT="" format type;

  case "$1" in
  -l) shift 1; type="log"; ;;
  -s) shift 1; type="scr"; ;;
  *)  type="all"; ;;
  esac

  for arg in "$@"; do TXTOUT+="$arg "; done
  TXTOUT="${TXTOUT%' '}"

  format="$TBCOLOR$BBCOLOR%-${WSLZ}s$NORMAL$BGDEF\n"
  TXTOUT=`echo -en "printf \"$format\" \"$TXTOUT\"" | zsh`

  if [[ "$type" == "all" || "$type" == "log" ]]; then echo -en "$TXTOUT\n" >> "$LOG"; fi
  if [[ "$type" == "all" || "$type" == "scr" ]]; then echo -en "$TXTOUT\n"; fi
}

################################################################
##
## outt printing function
##
## void psecho [-n] text, [-n] - new line after text :)
##
function psecho()
{
  local OPT="$1"
  local TXTMSG=""; local newln="NO"

  if [ "$OPT" == "-n" ]; then newln="YES"; shift; fi
  for arg in "$@"; do TXTMSG+="$arg"; done;
  slen=$(strlen "$TXTMSG")
  format="$TBCOLOR$BBCOLOR%-${WSLZ}s$NORMAL$BGDEF"
  TXTMSG=`echo -en "printf \"$format\" \"$TXTMSG\"" | zsh`
  getcursorxy
  gotoxy 1 $gY
  echo -en "$TXTMSG"
  if [ "$newln" == "YES" ]; then
	echo;
  else
	gotoxy $(($slen+2)) $gY;
  fi
}

################################################################
##
## Out text info
##
## void infoline [-l|-s] "text"
##
function infoline()
{
  local TXTOUT="" format type DT lz;
  gettimedate

  case "$1" in
  -l) shift 1; type="log"; ;;
  -s) shift 1; type="scr"; ;;
  *)  type="all"; ;;
  esac

  for arg in "$@"; do TXTOUT+="$arg "; done
  TXTOUT="${TXTOUT%' '}" # delete last ' '

  if [ $WORKTYPE == work ]; then lz=$(($WSLZ-19)); DT="$LDTTM"
  else lz=$(($WSLZ-22)); DT="░D░$LDTTM"; fi

  format="$TBTCOLOR$TBBCOLOR%-${lz}s"; setdefcolors; format+="$TBTCOLOR$TBBCOLOR%18s$STDCL$BGDEF"
  TXTOUT=`echo -en "printf \"$format\" \"$TXTOUT\" \"$DT\"" | zsh`

  if [[ "$type" == "all" || "$type" == "log" ]]; then echo -en "$TXTOUT\n" >> "$LOG"; fi
  if [[ "$type" == "all" || "$type" == "scr" ]]; then echo -en "$TXTOUT\n"; fi
  setdefcolors
}

################################################################
##
## helper for rita
##
## void datedayword void
##
function datedayword()
{
	let DATEW=`date +%d` # local date

	if (( $DATEW == 1 )); then DATEW="первое";
  elif (( $DATEW == 2 )); then DATEW="второе";
  elif (( $DATEW == 3 )); then DATEW="третье";
  elif (( $DATEW == 4 )); then DATEW="четвёртое";
  elif (( $DATEW == 5 )); then DATEW="пятое";
  elif (( $DATEW == 6 )); then DATEW="шестое";
  elif (( $DATEW == 7 )); then DATEW="седьмое";
  elif (( $DATEW == 8 )); then DATEW="восьмое";
  elif (( $DATEW == 9 )); then DATEW="девятое";
  elif (( $DATEW == 10 )); then DATEW="десятое";
  elif (( $DATEW == 11 )); then DATEW="одиннадцатое";
  elif (( $DATEW == 12 )); then DATEW="девенадцатое";
  elif (( $DATEW == 13 )); then DATEW="тринадцатое";
  elif (( $DATEW == 14 )); then DATEW="четырнадцатое";
  elif (( $DATEW == 15 )); then DATEW="пятнадцатое";
  elif (( $DATEW == 16 )); then DATEW="шестнадцатое";
  elif (( $DATEW == 17 )); then DATEW="семнадцатое";
  elif (( $DATEW == 18 )); then DATEW="восемнадцатое";
  elif (( $DATEW == 19 )); then DATEW="девятнадцатое";
  elif (( $DATEW == 20 )); then DATEW="двадцатое";
  elif (( $DATEW == 21 )); then DATEW="двадцать первое";
  elif (( $DATEW == 22 )); then DATEW="двадцать второе";
  elif (( $DATEW == 23 )); then DATEW="двадцать третье";
  elif (( $DATEW == 24 )); then DATEW="двадцать четвёртое";
  elif (( $DATEW == 25 )); then DATEW="двадцать пятое";
  elif (( $DATEW == 26 )); then DATEW="двадцать шестое";
  elif (( $DATEW == 27 )); then DATEW="двадцать седьмое";
  elif (( $DATEW == 28 )); then DATEW="двадцать восьмое";
  elif (( $DATEW == 29 )); then DATEW="двадцать девятое";
  elif (( $DATEW == 30 )); then DATEW="тридцатое";
  elif (( $DATEW == 31 )); then DATEW="тридцать первое";
	fi
}

################################################################
##
## Choicer
##
## void choiceOQ set RETCHS=0 - YES 1 - No !!!
##
## void choiceOQ prompt
##
function choiceOQ()
{
  local QUEST="$1" ch

  while true; do
	outt -s "$QUEST(y/n):"
	read -p "==> " ch 
	case $ch in
	y|Y) RETCHS=0; break;;
	n|N) RETCHS=1; break;;
	*) ;;
	esac
  done
}

################################################################
##
## sound system voice
## void sndsysoff void
##

################################################################
##
## # R)ечевой I)нформатор T)актической A)бстановки (RITA)
##
## rita voice printing function
##
## void rita text
##
function rita()
{
  if ! [ "$WORKTYPE" == "debug" ]; then
	if [ $RITAVOICE == ON ]; then
	  local TXTOUT="$1"
	  exec 7>/dev/tcp/$RITASERVER/$RITAPORT;
	  if [ -z $RITAVOLUME ]; then RITAVOLUME=70; fi
  	  echo -en "mixer vol $RITAVOLUME:$RITAVOLUME ;" >&7
  	  echo -en "echo \"$TXTOUT\" | RHVoice-test -p elena;" >&7
	  if (( `date +%k`>=$TMAF )); then echo -en "mixer vol $RITAVOLAFT:$RITAVOLAFT ;" >&7; fi
  	  echo "$TXTOUT" > "$RITALOG"
	  exec 7>&-
	fi
  fi
}

################################################################
##
## get zerosat sign calc
##
## getzerostat DB_FILE
##
function getzerostat()
{
  local DBNAME="$1"

  if [[ "$DBNAME" == "$INDEX_AVS" ]]; then
#	GSIGN="`date +%d`00" # local date
	GSIGN="0000" # local date
  elif [[ "$DBNAME" == "$INDEX_SND" || "$DBNAME" == "$INDEX_RCV" || "$DBNAME" == "$INDEX_DEL" || "$DBNAME" == "$INDEX_EML" ]]; then
	GSIGN="0"
  else
	GSIGN="`date +%d%m`00" # local date
  fi
}

################################################################
##
## get sign
##
## getsign DB_FILE
##
function getsign()
{
  local DBNAME="$1"
  local DATE=`date +%d%m` # local date

  if [ -f "$DBNAME" ]; then
    SAVEDATE=`cat "$DBNAME" | cut -d" " -f1 | cut -d":" -f2`
    if [ $DATE == $SAVEDATE ]; then
	  GSIGN=`cat "$DBNAME" | cut -d" " -f3 | cut -d":" -f2`
	else
	  getzerostat "$DBNAME"
	fi
  else
	getzerostat "$DBNAME"
  fi
}

################################################################
##
## update sign dbfile
##
## void updatedbfile DB_FILE date GSIGN SIGN
##
function updatedbfile()
{
  local DBFILE=$1 DATE=$2 GSIGN=$3 SIGN=$4;
  echo "Date:$DATE N:$GSIGN Sess:$SIGN " > $DBFILE
}

################################################################
##
## function setsign set global variable SIGN
##
## void setsign DB_FILE
##
function setsign()
{
  SIGN=""
  local DBFILE="$1" DATE SAVEDATE NS=1 TMH;

  if ! [ -z "$DBFILE" ]; then
	if [ "$DBFILE" == "$INDEX_AVS" ]; then
	  # for avos send time
	  let TMH=$((10#`date -u +%H`))  ## for convert 08 to 8 remove leader 0

	  if   (( $TMH==23 )) || (( $TMH==0 )); then TMH=00;
	  elif (( $TMH==5 ))  || (( $TMH==6 )); then TMH=06;
	  elif (( $TMH==11 )) || (( $TMH==12 )); then TMH=12;
	  elif (( $TMH==17 )) || (( $TMH==18 )); then TMH=18;
	  fi

	  DATE="`date +%d%m`" 	# get local date
	  SIGN="`date +%d`$TMH" # get local date
	  DT=`cat "$DBFILE" 2>/dev/null`
	  NS=`echo "$DT" | cut -d" " -f2 | cut -d":" -f2`
	  let NS++
	elif [ "$DBFILE" == "$INDEX_CLM" ]; then
	  DATE="`date +%d%m`" 		   # get local date
	  SIGN="`date -u -v-1m +%m`35" # get date month before  !!!
	  DT=`cat "$DBFILE" 2>/dev/null`
	  NS=`echo "$DT" | cut -d" " -f2 | cut -d":" -f2`
	  let NS++
	else
	  DATE=`date +%d%m`   # local date
	  DT=`cat "$DBFILE" 2>/dev/null`

	  SAVEDATE=`echo "$DT" | cut -d" " -f1 | cut -d":" -f2`
  	  if [ "$DATE" == "$SAVEDATE" ] ; then
	    NS=`echo "$DT" | cut -d" " -f2 | cut -d":" -f2`
	    let NS++
	  fi
	  if (( $NS < 10 )); then SIGN="$DATE"0"$NS";
	  else SIGN="$DATE$NS"; fi
	fi
	updatedbfile $DBFILE $DATE $NS $SIGN
  else
	setcolors $LRED $BBCOLOR
	outt "ERROR: setsign need set requare parameter!!!"
	setdefcolors
  fi
}

################################################################
##
## fmtcheck check valid format send messages
##
## void fmtcheck void
##
function fmtcheck()
{
  CONTROL=0
  local ret=0 fname="$FILESEND";

  if [ -f $fname ]; then

    F_SIGNATURE01=`head -n 1 "$FILESEND" | cut -d" " -f1 `
    F_SIGNATURE02=`head -n 1 "$FILESEND" | cut -d" " -f2 `

    if [ "$F_SIGNATURE01" == "#!AWR" ]; then return $ret; fi

    if [ "$SIGNATURE01" == "$F_SIGNATURE01" ] && [ "$SIGNATURE02" == "$F_SIGNATURE02" ]; then
	  ret=0
    else
	  fname="${fname##*/}" # delete all to last /
	  NAMEUNKNOWN=`date +%m%d-%H%M`-noformat-$fname # local date
	  mv -f "$fname" "$UNKNOWNDIR/$NAMEUNKNOWN"
	  settextcolor $LMAGENTA
	  outt "Файл $fname"
	  outt "Ошибка формата."
	  outt "Файл перемещён в $UNKNOWNDIR с именем:"
	  settextcolor $LGREEN
	  outt "$NAMEUNKNOWN"
	  setdefcolors
	  rita "Ошибка формата. Проверьте сигнатуру файла. Файл перемещён в каталог инфобокс."
	  ret=1;
    fi
  else
    txt="Файл для проверки не найден."
    outt "$txt"; rita "$txt"
    ret=1
  fi

  CONTROL=$ret
  return $ret;
}

################################################################
##
## sizecheck check size send mrssages
##
## void sizecheck void
##
function sizecheck()
{
  CONTROL=0
  local ret=0 fname="$FILESEND";

  if [ -f $fname ]; then
    let FSIZE=`stat -f %z "$FILESEND"`

    if (( $FSIZE >= $MINFSIZE )); then
	  ret=0
    else
      fname="${fname##*/}" # delete all to last /
	  NAMEUNKNOWN=`date +%m%d-%H%M`-nosize-$fname # local date
	  mv -f "$FILESEND" "$UNKNOWNDIR/$NAMEUNKNOWN"

	  settextcolor $LMAGENTA
	  outt "Файл $FILESEND"
	  outt "имеет слишком малый размер ($FSIZE). Проверьте содержимое файла."
	  outt "Файл перемещён в $UNKNOWNDIR с именем:"
	  settextcolor $LRED
	  outt "$NAMEUNKNOWN"
	  setdefcolors
      rita "Файл имеет малый размер. Проверьте файл. Файл перемещён в каталог инфобокс."
	  ret=1;
    fi
  else
    txt="Файл для проверки не найден"
    outt "$txt"; rita "$txt"
    ret=1
  fi

  CONTROL=$ret
  return $ret
}

################################################################
##
## tmcheckavos check valid time period for send AVOS messages
##
## void msgsendtimeavos void
## void msgsendtimeclim void
## void settmsin void but set NXTIMESIN or CRTIMESIN var
## void tmcheckavos void
##
function settmsin()
{ 
  local TMH="$1" TMM="$2"

  TMHM=$(($TMH*60+$TMM));

  local INT0020 INT0600 INT0620 INT1200 INT1220 INT1800 INT1820 INT2400 
  # SIN intervals
  INT0000=0           								
  INT0600=$((6*60));	INT0020=$SINTMAX;				# 00:20-06:00
  INT1200=$((12*60));	INT0620=$(( 6*60+$SINTMAX));	# 06:20-12:00
  INT1800=$((18*60));	INT1220=$((12*60+$SINTMAX));	# 12:20-18:00
  INT2400=$((24*60));	INT1820=$((18*60+$SINTMAX));	# 18:20-00:00

  CRTIMESIN="0"; NXTIMESIN="0";

    if (( (( $TMHM>$INT0020 )) && (( $TMHM<$INT0600 )) )); then NXTIMESIN="$SIN1"; 
  elif (( (( $TMHM>$INT0620 )) && (( $TMHM<$INT1200 )) )); then NXTIMESIN="$SIN2";
  elif (( (( $TMHM>$INT1220 )) && (( $TMHM<$INT1800 )) )); then NXTIMESIN="$SIN3";
  elif (( (( $TMHM>$INT1820 )) && (( $TMHM<$INT2400 )) )); then NXTIMESIN="$SIN0";

  elif (( (( $TMHM>=$INT0000 )) && (( $TMHM<=$INT0020 )) )); then CRTIMESIN="$SIN0"; 
  elif (( (( $TMHM>=$INT0600 )) && (( $TMHM<=$INT0620 )) )); then CRTIMESIN="$SIN1"; 
  elif (( (( $TMHM>=$INT1200 )) && (( $TMHM<=$INT1220 )) )); then CRTIMESIN="$SIN2"; 
  elif (( (( $TMHM>=$INT1800 )) && (( $TMHM<=$INT1820 )) )); then CRTIMESIN="$SIN3"; 
  fi

  if ! [ "$NXTIMESIN" == "0" ]; then TIMESIN="Следующий СИН-срок: $NXTIMESIN $STATION"; fi
  if ! [ "$CRTIMESIN" == "0" ]; then TIMESIN="Текущий СИН-срок: $CRTIMESIN $STATION"; fi
}

function msgsendtimeavos()
{
  local txt

  settextcolor $LRED; 
  txt="Отправка СИН осуществляется до СИН-срока."
  outt "$txt"; rita "$txt";

  settextcolor $LYELLOW; 
  outt "$TIMESIN" 

  settextcolor $LGREEN;  
  txt="Сохранённый СИН будет отправлен автоматически на следующем СИН-сроке."
  outt "$txt"; rita "Отложенная отправка";

  setdefcolors
}

function msgsendtimeclim()
{
  local txt
  outt " "
  settextcolor $LYELLOW; setbackcolor $BGRED
  txt=" Соообщение тип CLIMAT создаётся поздно по времени!"
  outt "$txt"; rita "$txt";
  setdefcolors
}

function tmcheckavos()
{
  CONTROL=0
  local ret=0 txt TMH TMM TMDM TYPEMSG="$1" OUTSCREEN=$2;

  if [[ $TYPEMSG == "avos" ]]; then

	# for avos!!! UTC!
	TMH=$((10#`date -u +%H`))  ## for convert 08 to 8 remove leader 0 UTC hours
	TMM=$((10#`date -u +%M`))  ## ... too UTC min's

	#TMH=18; TMM=10;

	# 00:00-00:20 06:00-06:20 12:00-12:20 18:00-18:20 UTC
	if((
		(( (( $TMH==0 )) || (( $TMH==6 )) || (( $TMH==12 )) || (( $TMH==18 )) ))
		&&
		(( (( $TMM>=$SINTMIN )) && (( $TMM<=$SINTMAX )) ))
	  )); then
  	  ret=0
  	  settmsin "$TMH" "$TMM"
	else
  	  settmsin "$TMH" "$TMM"
	  if ! [[ "$OUTSCREEN" == "SILENT" ]]; then
		TMST=$(date "+%Y.%m.%d %H:%M:%S")
  		if [ -f "$FILESEND" ] ; then
		  sudo mv -f "$FILESEND" "$LDIRDELAY/"
		  echo "Создан: $USER $TMST" > "$SIGN_DELAY"
		fi
		msgsendtimeavos
	  fi
  	  ret=1                      
	fi
  elif [[ $TYPEMSG == "climat" ]]; then
	TMDM=$((10#`date -u +%d`))  ## ... too UTC min's daymonth
	#TMDM=0;
	if(( $TMDM <= 3 )); then
	  ret=0
	else
	  msgsendtimeclim
	  if ! [[ "$OUTSCREEN" == "SILENT" ]]; then
		settextcolor $LGREEN;
		str="Отправить сообщение типа CLIMAT?(y/n)=> "
		while true; do
		  psecho "$str"; read chs
		  case $chs in
		  y|Y) ret=0; RETCHS=0
			 break 
		  ;;
		  n|N) outt "Cообщение типа CLIMAT удалено."
			 sudo rm $LDIRFORSEND/avosclimat.txt 
			 ret=1; RETCHS=1
			 break 
		  ;;  
		  *)
		  ;;
		  esac
		done
	  else
		ret=1; RETCHS=1
	  fi # SILENT
	fi
  fi
  setdefcolors
  CONTROL=$ret
return $ret
}

################################################################
##
## cparchfiles archiving send messages
##
## void cparchfiles dirsave newnamesendmsg [vos|svc|avos].txt]
##
function cparchfiles()
{
  local DIRSAVE="$1" NAMESENDMSG="$2" TYPELETTER="$3"

  if ! [ -d "$LDIR_ARCH/$DIRSAVE/$ARCHDIR" ]; then crdsetpm "$LDIR_ARCH/$DIRSAVE/$ARCHDIR"; fi
  if ! [ -d "$RDIR_ARCH/$DIRSAVE/$ARCHDIR" ]; then crdsetpm "$RDIR_ARCH/$DIRSAVE/$ARCHDIR"; fi

  cp -f "$NAMESENDMSG" "$LDIR_ARCH/$DIRSAVE/$ARCHDIR/"
  cp -f "$NAMESENDMSG" "$RDIR_ARCH/$DIRSAVE/$ARCHDIR/"

  sudo chmod -Rf $perm  "$LDIR_ARCH/$DIRSAVE/$ARCHDIR" &> /dev/null
  sudo chown -Rf $owner "$LDIR_ARCH/$DIRSAVE/$ARCHDIR" &> /dev/null
  sudo chmod -Rf $perm  "$RDIR_ARCH/$DIRSAVE/$ARCHDIR" &> /dev/null
  sudo chown -Rf $owner "$RDIR_ARCH/$DIRSAVE/$ARCHDIR" &> /dev/null

  if [[ "$DIRSAVE" == "OutBox" && -n $TYPELETTER ]]; then
	if ! [ -d "$LDIR_ARCH/Send/$ARCHDIR" ]; then crdsetpm "$LDIR_ARCH/Send/$ARCHDIR"; fi
	if ! [ -d "$RDIR_ARCH/Send/$ARCHDIR" ]; then crdsetpm "$RDIR_ARCH/Send/$ARCHDIR"; fi

	cp -f "$NAMESENDMSG" "$LDIR_ARCH/Send/$ARCHDIR/"
	cp -f "$NAMESENDMSG" "$RDIR_ARCH/Send/$ARCHDIR/"

	sudo chmod -Rf $perm  "$LDIR_ARCH/Send/$ARCHDIR/" &> /dev/null
	sudo chown -Rf $owner "$LDIR_ARCH/Send/$ARCHDIR/" &> /dev/null
	sudo chmod -Rf $perm  "$RDIR_ARCH/Send/$ARCHDIR/" &> /dev/null
	sudo chown -Rf $owner "$RDIR_ARCH/Send/$ARCHDIR/" &> /dev/null
  fi

  if [[ -f "$TYPELETTER" && -n $TYPELETTER ]]; then rm "$TYPELETTER" &> /dev/null; fi
}

################################################################
##
## AVOS prepare AVOSDAY/AVOSCLIMAT
##
## void AddFnamePath localpathname remotepathname|e-mail subj txtmail 
##
function AddFnamePath()
{
  local LPFNAME="$1" RMPTHNAME="$2"  SUBJECT="$3"; TXTMAIL="$4";

  WLFNPTH[$nFLSND]="$LPFNAME"
  WRFNPTH[$nFLSND]="$RMPTHNAME"
  WSUBJCT[$nFLSND]="$SUBJECT"
  WLETTXT[$nFLSND]="$TXTMAIL"
  let nFLSND++
}

################################################################
##
## getmonth
##
## void getmonth void
##
function getmonth()
{
	mnth=`date -v-1m +%m`

	  if [ "$mnth" == "01" ]; then echo -en "январь";
	elif [ "$mnth" == "02" ]; then echo -en "февраль";
	elif [ "$mnth" == "03" ]; then echo -en "март";
	elif [ "$mnth" == "04" ]; then echo -en "апрель";
	elif [ "$mnth" == "05" ]; then echo -en "май";
	elif [ "$mnth" == "06" ]; then echo -en "июнь";
	elif [ "$mnth" == "07" ]; then echo -en "июль";
	elif [ "$mnth" == "08" ]; then echo -en "август";
	elif [ "$mnth" == "09" ]; then echo -en "сентябрь";
	elif [ "$mnth" == "10" ]; then echo -en "октябрь";
	elif [ "$mnth" == "11" ]; then echo -en "ноябрь";
	elif [ "$mnth" == "12" ]; then echo -en "декабрь";
	  fi
}

function sendcancel()
{
    local msg="$1"
    settextcolor $LYELLOW; setbackcolor $BGRED
    outt "$msg"
    setdefcolors
}

################################################################
##
## AVOS prepare AVOSDAY/AVOSCLIMAT
##
## void make_METEO maskfile signature dbfile
##
function make_METEO()
{
  local TYPEMSG="$1" SIGNATURE="$2" DBFILE="$3";
  local EMAIL TXTMAIL;

  if [ "$TYPEMSG" == "avos" ]; then
	FILESEND="$LDIRFORSEND/$TYPEMSG.txt"
  elif [ "$TYPEMSG" == "climat" ]; then
	FILESEND="$LDIRFORSEND/avos$TYPEMSG.txt"
  fi
  
  if [ -f $FILESEND ]; then
	SIGNATURE01="$SIGNATURE"; SIGNATURE02=$METEOSIGNATURE

	if fmtcheck && sizecheck && tmcheckavos $TYPEMSG; then CONTROL=0; else CONTROL=1; fi # if (( ARG )) - 1(TRUE) 0(FALSE)

	if (( ! $CONTROL )); then
	  setsign "$DBFILE"
	  fnsend="avos$SIGN.txt";
	  echo $fnsend > "$LASTMT"
	  cp "$FILESEND" "$LDIRSEND/$fnsend"

	  cparchfiles Meteo "$LDIRSEND/$fnsend" "$FILESEND"

	  #for ASPD /astd
	  AddFnamePath "$LDIRSEND/$fnsend" "$FTPSTDPATHSEND"
#	  AddFnamePath "$LDIRSEND/$fnsend" "/rae/vos"

	  #for PROGRESS avos climat and emailing
	  if [ "$TYPEMSG" == "climat" ]; then
		AddFnamePath "$LDIRSEND/$fnsend" "/rae/prg"

		if [ $WORKTYPE == debug ]; then EMAIL="admin_vostok@aari.ru";
		else EMAIL="meteo@progress.aari.ru";fi

		TXTMAIL="Примите файл климата от станции $STATION за `getmonth` `date +%Y`."
		AddFnamePath "$LDIRSEND/$fnsend" "$EMAIL"  "Климат" "$TXTMAIL"
	  fi
	fi
      a=1
#    sendcancel "Отмена отправки метео-сообщения $FILESEND !"
  fi
}

################################################################
##
## stdvrstxt genegate std subletter
##
## void stdvrstxt letterout namevrs SIGNVRS
##
function stdvrstxt()
{
  local LETTER LETTERNAME=$1 NAMEVRS=$2 NSIGN=$3;

  LETTER="СЖЦ ВОСТОК ШРМ `date -u -v+3H '+%d/%m %H%M'`=\r\n" # get ASPD(MSK) date
  LETTER+="АСПД НС=\r\n\r\n"
  LETTER+="ПРОШУ ПОДТВЕРДИТЬ ПРИЁМ ${NAMEVRS^^} FTP ИЛИ EMAIL\r\n"
  LETTER+="\r\nСОДЕРЖИТ:\r\n`ls -1 \"$LDIRFORSEND\" | egrep -v 'vrs.txt'`\r\n";
  LETTER+="\r\nУВАЖЕНИЕМ=\r\n"
  LETTER+="$NSIGN ВОСТОК ШРМ-\r\n"
  echo -en "$LETTER" > $LETTERNAME
}

################################################################
##
## vrs helper
##
## void messagePrepVRS void
##
function messagePrepVRS()
{
  outt "Подписной # $SIGN Письмо: $VRSSNM Формат архива: $ARCHTYPE."
  szbefore=`du -hs "$LDIRFORSEND" | cut -f1`
  outt "Имя файла: $NAMEARCHLT"
}

################################################################
##
## parceletter - genetate messages VOS SVC VRS
## and set status adress (ON/OFF)
##
## void parceletter lettesrc namesend sign [archives file]
##

function prepattachname()
{
    local strlet="$1" atname="" atnameRN="" email="$2"

    if [ -z $email ]; then email=false;
    else email=true; fi

    atname="$(echo "$strlet" | cut -d":" -f2-)"

    if ! [ -z "${atname##*$'/'*}" ]; then atnameNN="$LDIRFORSEND/$atname"
    else atnameNN=$atname; fi

    if [[ -f "$atnameNN" ]]; then 
        atnameRN="${atnameNN//' '/'_'}"; #replace
        cparchfiles OutBox "$atnameNN" 

        if $email; then atnameRN="$MATTACHDIR/${atnameRN##*/}" 
        else atnameRN="$TMPSTORE/${atnameRN##*/}"
        fi

        if ! [ -z "${atname##*$'/'*}" ]; then mv "$atnameNN" "$atnameRN" &> /dev/null
        else cp "$atnameNN" "$atnameRN" &> /dev/null; fi

        echo -en "$atnameRN"
    else
        echo -en "None"
    fi
}

function parceletter()
{
  local LTYPE="$1" LETNAME="$2" NAMESEND="$3" NSIGN="$4" ARCH_PARTS=($5) F_SIGNATURE=""
  local fndadr=false LETATCH=false CHEEFLINE=false GRECV=false
  local strlet i iat=0 ATCH="" il=0 ia=0 is=0 atname; 
  declare -a ADRESS LBODY LSIGN

  if [[ "$LTYPE" == "VRS" ]]; then
	NAMEARCHLT=$NAMESEND
	NAMESEND=$LETNAME
	LETNAME="$LDIRFORSEND/vrs.txt"

	if ! [ -f "$LETNAME" ]; then
	  txt="Отправляется стандартный сопроводительный файл."
	  rita "$txt"; outt "$txt";
	  stdvrstxt "$NAMESEND" "$NAMEARCHLT" "$NSIGN"
	  return
	fi
  else
    F_SIGNATURE=`head -n 1 "$LETNAME" | cut -d" " -f1 `  
  fi

  dl=0
  while read -r -u5 strlet ; do
	fndadr=false
	if ! [ -z "${strlet##$'#'*}" ]; then

        if [ -z "${strlet##$"CHEEF:"}" ]; then CHEEFLINE=true; continue;  fi
	    if [ -z "${strlet##$"GRECV:"}" ]; then GRECV=true; continue;  fi 

	    if ! [ -z "${strlet##$'!'*}" ]; then
		    # Prepare adresses
            if [ -z "${strlet##$"ATTACH:"*}" ]; then
                echo "00A: strlet=$strlet"
                atname=$(prepattachname "$strlet")
                echo "00B: atname=$atname"
                if [[ "$atname" == "None" ]]; then continue; fi
                PTFATCH[$iat]="$atname"
                NMFATCH[$iat]="${atname##*/}"
		        LETATCH=true
		        let iat++
	        elif [ -z "${strlet##$'@'*}" ]; then 
		        strlet="${strlet#"@"}" # remove first '@'
		        if [ -z "${strlet##$"E-MAIL:"*}" ]; then
			        fndadr=true
			        sdel0="e-mail:"; sdel1="${sdel0^^}"
			        email="$(echo "$strlet" | cut -d";" -f1)"
			        email=${email//$sdel0}; email=${email//$sdel1}
			        SUBJ="$(echo "$strlet" | cut -d";" -f2 | cut -d":" -f2)"
			        TXTMAIL="Cообщение от станции $STATION.\n"
			        TXTMAIL+="$(echo "$strlet" | cut -d";" -f3 | cut -d":" -f2)"
			        ADRESS[$ia]="$email / $SUBJ=\r\n"; let ia++

			        if [[ "$LTYPE" != "VRS" ]]; then ATCH="$NAMESEND"; fi
            
                    if [ -z "${strlet##*$'ATTACH:'*}" ]; then
                        ATTSTR="$(echo "$strlet" | cut -d";" -f4 | cut -d":" -f2)"
                        OLDIFS="$IFS"; IFS=','
	                    for atn in $ATTSTR; do
                            echo "00C: atn=$atn"
                            atname=$(prepattachname "$atn" 'email')
                            echo "00D: atname=$atname"
                            if [[ "$atname" == "None" ]]; then continue; fi
                            PTFATCH[$iat]="$atname"
                            NMFATCH[$iat]="${atname##*/}"
                            ATCH+=" $atname "  
	                    done
	                    IFS="$OLDIFS"
                    fi    

			        if (( $iat > 0 )); then
			            for (( j=0; j<$iat; j++ )); do
			                ATCH+=" ${PTFATCH[$j]}"
			            done
			        fi

			        if [[ "$LTYPE" == "VRS" ]]; then
			            count=${#ARCH_PARTS[@]}
			            if (( count > 0 )); then
            	            for (( j=0; j<$count; j++ )); do
				                ATCH+=" ${ARCH_PARTS[$j]}"
				            done
			            fi
                    fi

			        AddFnamePath "$ATCH" "$email" "$SUBJ" "$TXTMAIL" 
		        else
			        SUBJ="$(echo "$strlet" | cut -d";" -f2 | cut -d":" -f2-)"
                    strl="$(echo "$strlet" | cut -d";" -f1)"
			        for (( i=0; i<$COUNTADRESS; i++ )); do
			            if [ -z "${strl##${WADRESS[$i]}}" ]; then
				            fndadr=true
                            if [ -z "${WADRESS[$i]##*$"e-mail"*}" ]; then ADRESS[$ia]="${WADRESS[$i]}:${WFTPPTH[$i]}=\r\n"; let ia++
                            else ADRESS[$ia]="${WADRESS[$i]^^}=\r\n"; let ia++
                            fi

				            if [[ "$LTYPE" != "VRS" ]]; then 
                                AddFnamePath "$NAMESEND" "${WFTPPTH[$i]}" "$SUBJ";
				                if (( $iat > 0 )); then
					                for (( j=0; j<$iat; j++ )); do
					                    AddFnamePath "${PTFATCH[$j]}" "${WFTPPTH[$i]}" "$SUBJ";
					                done
				                fi
				            fi
				            break
			            fi
			        done
			        if ! "$fndadr"; then
			            fndadr=true
			            LBODY[$il]="${strl^^}=\r\n"; let il++
                    fi
		        fi # e-mail
            fi # @
        fi # if for search NOT "!"

	    # Prepare other text
	    if ! "$fndadr"; then
            # srtlet put in letter as is delete '!'
		    if [ -z "${strlet##$'!'*}" ]; then LBODY[$il]="${strlet#"!"}\r\n";   
		    else LBODY[$il]="${strlet^^}\r\n";
		    fi
		    let il++
	    fi
    fi
  done 5< "$LETNAME"

  if [[ "$LTYPE" == "VRS" ]]; then
      TMPSTR="СОДЕРЖИТ:\r\n`ls -1 \"$LDIRFORSEND\" | egrep -v 'vrs.txt'`\r\n"
      TMPSTR=`echo "$TMPSTR" | awk 'sub("$", "\r")'`
      LBODY[$il]="$TMPSTR"; let il++
      GRECV=true
  fi

  if $GRECV; then LBODY[$il]="\r\nПРОШУ ПОДТВЕРДИТЬ ПРИЁМ ${NAMEARCHLT^^} FTP ИЛИ EMAIL СПС\r\n"; let il++; fi
  
  if $LETATCH ; then
    LBODY[$il]="\r\nПримите файлы:\r\n"; let il++
	for (( j=0; j<$iat; j++ )); do
	  LBODY[$il]=" ${NMFATCH[$j]}\r\n"; let il++
   	done
  fi

  LSIGN[$is]="\r\nУВАЖЕНИЕМ=\r\n";let is++
  if $CHEEFLINE; then LSIGN[$is]="$NSIGN $STATION $CHEEF-\r\n"; let is++; 
  else LSIGN[$is]="$NSIGN $STATION ШРМ-\r\n"; let is++
  fi

  # Add SJC
  SJC="СЖЦ ВОСТОК ШРМ `date -u -v+3H '+%d/%m %H%M'`=\r\n" # MSK date!!! Time ASPD !!!
  echo -en "$SJC" > "$NAMESEND"

  # Add ADRESS
  if [[ "$F_SIGNATURE" != "#!AWR" ]]; then
    for (( i=0; i<$ia; i++ )); do echo -en "${ADRESS[i]}" >> "$NAMESEND"; done
  fi
  echo -en "\r\n" >> "$NAMESEND";
  
 #Add LBODY
  for (( i=0; i<$il; i++ )); do echo -en "${LBODY[i]}" >> "$NAMESEND";  done
  echo -en "\r\n" >> "$NAMESEND";

  #Add LSIGN
  for (( i=0; i<$is; i++ )); do echo -en "${LSIGN[i]}" >> "$NAMESEND";  done  
  echo -en "\r\n" >> "$NAMESEND";
}

################################################################
##
## VOS/SVC prepare
##
## void make_VOS_SVC filemask signature dbfile
##
function make_VOS_SVC()
{
  local FMASK="$1" SIGNATURE="$2" DBFILE="$3"
  local ret=0; local count;

  FILES_SEND=($LDIRFORSEND/$FMASK*.txt)
  count=${#FILES_SEND[@]}

  if [ -f ${FILES_SEND[0]} ]; then

	SIGNATURE01="$SIGNATURE"; SIGNATURE02='RADIO';

  	for (( i=0; i<$count; i++ )) ; do
	  FILESEND=${FILES_SEND[$i]}
	  if fmtcheck && sizecheck; then
		setsign "$DBFILE"; fnsend="$FMASK$SIGN.txt"
		parceletter "VOS_SVC" "$FILESEND" "$LDIRSEND/$fnsend" $SIGN
		sudo chmod $perm "$LDIRSEND/$fnsend"
		sudo chown $owner "$LDIRSEND/$fnsend"
        cparchfiles OutBox "$LDIRSEND/$fnsend" "${FILES_SEND[$i]}"
      else
        sendcancel "Отмена VOS/SVC-отправки: $FILESEND !"
	  fi
    done # i
  fi
}

################################################################
##
## sumdirsize
##
## void sumdirsize pathdir
##
function sumdirsize()
{
  local DIRNSZ="$1" SZBL;
  SZBL=`du -s "$DIRNSZ" | cut -f1`;
  echo -en "$(($SZBL*512))"
}

################################################################
##
## repare VRS archiver
##
## void archivesvrs VRSADD
##
function archivesvrs()
{
  local VRSADD="$1";

  # Copy VRS-letter to OutBox
  if ! [ -d "$LDIR_ARCH/OutBox/$ARCHDIR" ]; then crdsetpm "$LDIR_ARCH/OutBox/$ARCHDIR"; fi
  if ! [ -d "$RDIR_ARCH/OutBox/$ARCHDIR" ]; then mkdir -p "$RDIR_ARCH/OutBox/$ARCHDIR"; fi

  cp "$VRSADD" "$LDIR_ARCH/OutBox/$ARCHDIR/"
  cp "$VRSADD" "$RDIR_ARCH/OutBox/$ARCHDIR/"

  # Copy all to Archive OutBox
  if ! [ -d "$LDIR_ARCH/Send/$ARCHDIR" ]; then crdsetpm "$LDIR_ARCH/Send/$ARCHDIR"; fi
  if ! [ -d "$RDIR_ARCH/Send/$ARCHDIR" ]; then crdsetpm "$RDIR_ARCH/Send/$ARCHDIR"; fi

  if [ -n "`ls $LDIRFORSEND`" ]; then
    cp -R "$LDIRFORSEND"/* "$LDIR_ARCH/OutBox/$ARCHDIR/"
    cp -R "$LDIRFORSEND"/* "$RDIR_ARCH/OutBox/$ARCHDIR/"
    rm -R "$LDIRFORSEND"/*
  fi

  cp $LDIRSEND/*$ARCHTYPE* "$LDIR_ARCH/Send/$ARCHDIR/"
  cp $LDIRSEND/*$ARCHTYPE* "$RDIR_ARCH/Send/$ARCHDIR/"
  cp $VRSADD "$LDIR_ARCH/Send/$ARCHDIR/"
  cp $VRSADD "$RDIR_ARCH/Send/$ARCHDIR/"
}

################################################################
##
## repare VRS and Other
##
## void make_VRS void
##
function sfq()
{
    local SFQ numparts=$1

    if (( $numparts<10 )); then SFQ="00$numparts"    
    elif (( $numparts>=10 )); then SFQ="0$numparts"
    elif (( $numparts>=100 )); then SFQ="$numparts"
    fi
    echo -en "$SFQ"
}

################################################################
##
## repare VRS and Other
##
## void make_VRS void
##
function make_VRS()
{
  local ret=0 ARCHNAME LETTERINFO NAMEARCHLT txt

  if [ -n "`ls \"$LDIRFORSEND\"`" ] ; then

	if ! [ -d "$LDIR_ARCH/OutBox/$ARCHDIR" ]; then crdsetpm "$LDIR_ARCH/OutBox/$ARCHDIR"; fi
    if ! [ -d "$RDIR_ARCH/OutBox/$ARCHDIR" ]; then crdsetpm "$RDIR_ARCH/OutBox/$ARCHDIR"; fi

	if [ -f "$INFODIR/*.Next" ]; then rm "$INFODIR/*.Next"; fi

	# Calculate sign for send
	FILESEND="$LDIRFORSEND/vrs.txt"
	FILETMP="$TMPASPD/vrs.txt"
    SIGNATURE01='#!VRS'; SIGNATURE02='RADIO';

	if ! fmtcheck; then 
      sendcancel "Отмена VRS-отправки:"
      sendcancel "$FILESEND !"
      return; 
    fi

	mv -f "$FILESEND" "$FILETMP"

	getfsinfo "$LDIRFORSEND/*"
	fslist "$FLLSLOG" "Файлы для VRS отправки:"

	setsign "$INDEX_VRS"
	LETTERINFO="$LDIRFORSEND/vrs$SIGN.txt"
	ARCHNAME="vrs$SIGN.$ARCHTYPE"

	# Get size of files in directory before compressed
	if (( $(sumdirsize "$LDIRFORSEND")>=$FILEPACKSIZE )); then
	  BIGSIZE=ON
	  rita "Внимание! Большой размер архива. Несколько частей. Проверьте его отправку!"
	  settextcolor $LRED
	  outt "Многотомный архив! Проверьте отправку!"
	  setdefcolors

	  if [ $ARCHTYPE == rar ]; then
		rar a -ep -m5 -v$ARCHSIZE "$LDIRSEND/$ARCHNAME" "$LDIRFORSEND/*"  >> /dev/null
		ARCH_PARTS=("$LDIRSEND"/vrs$SIGN.part*)
		NAMEARCHLT="vrs$SIGN.part1-${#ARCH_PARTS[@]}.$ARCHTYPE" # !!!!!
		messagePrepVRS
        # Разбор cопроводительного письма
        mv -f "$FILETMP" "$FILESEND" 
		parceletter "VRS" "$LETTERINFO" "$NAMEARCHLT" $SIGN "$LDIRSEND/vrs$SIGN.part*" 
	  else
		7z a -t$ARCHTYPE -v$ARCHSIZE "$LDIRSEND/$ARCHNAME" "$LDIRFORSEND/*" >> /dev/null
		ARCH_PARTS=("$LDIRSEND"/vrs$SIGN.$ARCHTYPE.*)
		NAMEARCHLT="vrs$SIGN.$ARCHTYPE.001-$(sfq ${#ARCH_PARTS[@]})" # !!!!!
		messagePrepVRS
        # Разбор cопроводительного письма
        mv -f "$FILETMP" "$FILESEND" 
		parceletter "VRS" "$LETTERINFO" "$NAMEARCHLT" $SIGN "$LDIRSEND/vrs$SIGN.$ARCHTYPE.*"
	  fi
	  rm -f "$FILESEND"
      mv "$LETTERINFO"  "$LDIRSEND/"
      LETTERINFO="$LDIRSEND/vrs$SIGN.txt"
      AddFnamePath "$LETTERINFO" "$FTPSTDPATHSEND"
	  for (( i=0; i<${#ARCH_PARTS[@]}; i++ )) ; do
		AddFnamePath "${ARCH_PARTS[$i]}" "$FTPSTDPATHSEND"
	  done # i
	else
	  NAMEARCHLT=$ARCHNAME
	  messagePrepVRS
      # Разбор cопроводительного письма
      mv -f "$FILETMP" "$FILESEND" 
	  parceletter "VRS" "$LETTERINFO" "$ARCHNAME" "$SIGN" "$LDIRSEND/$ARCHNAME"
      mv "$LETTERINFO"  "$LDIRSEND/"
      LETTERINFO="$LDIRSEND/vrs$SIGN.txt"
      AddFnamePath "$LETTERINFO" "$FTPSTDPATHSEND"
	  rm -f "$FILESEND"
	  if [ $ARCHTYPE == rar ]; then
		rar a -ep -m5 "$LDIRSEND/$ARCHNAME" "$LDIRFORSEND/*" >> /dev/null
	  else
		7z a -t$ARCHTYPE "$LDIRSEND/$ARCHNAME" "$LDIRFORSEND/*" >> /dev/null
	  fi
	  AddFnamePath "$LDIRSEND/$ARCHNAME" "$FTPSTDPATHSEND"
	fi

	# get size after compressed
	outt "Объём файлов: $szbefore. Объём отправки: `du -hs \"$LDIRSEND\" | cut -f1`"
	setdefcolors
	archivesvrs "$LETTERINFO"
  fi
}

################################################################
##
## ftpsend helper
##
## void checkrptpath void
##
function GenCmdSend()
{
  local i j PUTCMD LSTCMD;
  local saveLFname="" saveRFpath="" im oi=0 NMM;
  local ML=$(( ($WSLZ-42)/2 )) cml=0;

  if (( $nFLSND > 0 )); then

	FTPCMD="$FTPCMDOPEN"

	for (( i=0,im=0; i<$nFLSND; i++ )); do
	  # droped repeate adresses and files
	  if ! [[ "$saveLFname" == "${WRFNPTH[$i]}" && "$saveRFpath" == "${WRFNPTH[$i]}" ]]; then
		if [ -z "${WRFNPTH[$i]##*$'@'*}" ]; then

		  let nMAILSND++
		  WMEMAILAD[$im]="${WRFNPTH[$i]}"
		  WMSUBJECT[$im]="${WSUBJCT[$i]}"
		  WMTXTMAIL[$im]="${WLETTXT[$i]}"
		  WMATTACHM[$im]="${WLFNPTH[$i]}"
		  let im++
		else
		  let nFTPSND++
		  NMM="${WLFNPTH[$i]##*/}" 			# последнее слово в path
		  PUTCMD+="rm -f \"${WRFNPTH[$i]}/$NMM\";"
		  PUTCMD+="put -c -O \"${WRFNPTH[$i]}\" ${WLFNPTH[$i]};"
          
#		  PUTCMD+="chmod 666 ${WRFNPTH[$i]}/$NMM;"
		  saveLFname="${WLFNPTH[$i]}"; saveRFpath="${WRFNPTH[$i]}";
		fi
	  fi
	done

	echo "${WRFNPTH[0]}" | grep -v "@" > "$TMP"
	for (( i=1; i<${#WRFNPTH[@]}; i++ )); do
	  echo "${WRFNPTH[$i]}" | grep -v "@" >> "$TMP"
	done

	for ftppath in `sort -u "$TMP"`; do
	  LSTCMD+="echo \"$ftppath\";rels \"$ftppath\";"
	done

	FTPCMD+="$PUTCMD$LSTCMD$FTPCMDCLOSE"
  else
	outt "Нет файлов для отправки"
  fi

#echo "========================="
#echo "$FTPCMD"
#echo "========================="
#exit
}

################################################################
##
## ftp sender helper
##
## void maxsend void
##
function maxsend()
{
  settextcolor $LMAGENTA
  outt "Макс. кол-во ($MAXRPT) попыток. Проверьте соединение"
  setdefcolors
  rita "Максимальное количество попыток. Проверьте соединение. Отправка отменена."
}

################################################################
##
## ftp sender
##
## void ftpsend ldir
##
function ftpsend()
{
  local LDIR="$1" i;

  if (( $nFTPSND > 0 )); then

	infoline "Отправка ftp"
	getfsinfo "$LDIR/*"

	for (( i=1; i<=$MAXRPT; i++ )); do

	  lftp -c "$FTPCMD" &> "$FTPSLOG"

	  if fseqsize "Отправка. АСПД" "$FTPSLOG"; then
		infoline "Файлы для АСПД отправлены #$i($MAXRPT)"
		rita "Файлы для А эС Пэ Дэ отпрaвлены. Размеры файлов совпадают."
		fslist "$FTPSLOG" "Отправка на АСПД" "FTPFS"
		break;
	  else
		settextcolor $LMAGENTA
		outt "Повторная отправка файла #$i($MAXRPT)"
		setdefcolors
		rita "Повторная отправка. Попытка номер $i."
	  fi
	done #i
	if (( $i>=$MAXRPT )); then maxsend; fi
  fi
  nFTPSND=0;
}

################################################################
##
## ftp sender once
##
## int ftpsendonce file
##
##############################################################
function ftpsendonce()
{
  local i FILESEND="$1";
  infoline "Отправка ftp"

  lfile="${FILESEND##*/}"
  lpath="${FILESEND%/*}"

  if [ -z "$lpath" ]; then lpath="./"; fi

  outt "Файл: $FILESEND"
  getfsinfo "$FILESEND"

  for (( i=1; i<=$MAXRPT; i++ )); do
    infoline "Попытка #$i($MAXRPT)"

    lftp -c "$FTPCMD" &> "$FTPSLOG"

    if fseqsize "Отправка \"$FILESEND\"" "$FTPSLOG"; then
  	  infoline "Успешная отправка #$i($MAXRPT)"
	  rita "Отправлены файлы для А эС Пэ Дэ. Размеры файлов совпадают."
	  fslist "$FTPSLOG" "Отправка. АСПД \"$FILESEND\"" "FTPFS"
	  break;
	else
	  settextcolor $LMAGENTA
	  outt "Повторная попытка #$i($MAXRPT)"
	  setdefcolors
	  rita "Повторная потытка номер $i из $MAXRPTх"
	fi
  done
  if (( $i>=$MAXRPT )); then maxsend; fi
}

################################################################
##
## mail sender
##
## int emailsend dir file
##
function emailsend()
{
  local i im=${#WMEMAILAD[@]};

  if (( $im == 0 )); then return 0; fi

  local lz lnout lntabbg lntabend lnatch lnatend ATTACH word="файл" i j k;
  local lza lzs FILENOT=OFF tmst;

  txt="Отправка e-mail"
  infoline "$txt"; rita "$txt";
  gettimedate
  lz=$(($WSLZ-2)); tmst=${#LDTTM}; lza=$((lz-(9+$tmst)));

  lntabbg="┌"; lntabbg+=`printf "─%.s" $(seq $lz)`;lntabbg+="┐"; outt "$lntabbg";
  lnintol="├"; lnintol+=`printf "─%.s" $(seq $lz)`;lnintol+="┤"

  fmterro="│$ETCOLOR$EBCOLOR%-${lz}s$TBCOLOR$BBCOLOR│"
  fmtadrs="│ E-mail: $FLSCOLOR$BBCOLOR%-${lza}s$TBCOLOR$BBCOLOR%${tmst}s│"
  fmtsubj="│Subject: $BDCOLOR$BBCOLOR%-${lza}s$TBCOLOR$BBCOLOR%${tmst}s│"
  fmtatch="│ Attach: $PBCOLOR$BBCOLOR%-${lza}s$TBCOLOR$BBCOLOR%${tmst}s│"
  fmtline="│ =====>: $GRAY$BBCOLOR%-${lza}s$TBCOLOR$BBCOLOR%${tmst}s│"

  for (( i=0; i<$im; i++ )); do
	gettimedate
	# Вывод адреса
	lnout=`echo -en "printf \"$fmtadrs\" \"${WMEMAILAD[$i]}\" \"$LDTTM\"" | zsh`; outt "$lnout";

	# Вывод сабжа
	if [ -z "${WMSUBJECT[$i]}" ]; then SUBJ="$STATION Station"
	else SUBJ="${WMSUBJECT[$i]}"; fi

	lnout=`echo -en "printf \"$fmtsubj\" \"$SUBJ\" \"$LDTTM\"" | zsh`; outt "$lnout";

	# Вывод аттачей
	if (( ${#WMATTACHM[@]} > 0 )); then
	  declare -a ATTCHS

	  ATTACH=""; k=0; OLDIFS="$IFS"; IFS=' ';
	  for NM in ${WMATTACHM[$i]}; do
		if [ -z "`ls $NM 2>/dev/null`" ] ; then
		  ERROR="Файл $NM не найден"
		  FILENOT=ON
		  break
		else
		  ATTACH+=${NM##*/}  ATTACH+=" ";
		  let k++
		fi
	  done
	  IFS="$OLDIFS"

	  if [ $FILENOT == ON ]; then
		lnout=`echo -en "printf \"$fmterro\" \"Ошибка. $ERROR. Отмена\"" | zsh`; outt "$lnout";
		break;
	  fi

	  if (( $k>1 )); then word="файлы"; fi

	  k=0; lnfull=${#ATTACH}; LNS=$((lz-20)); str=""
	  for (( j=0,k=0; j<$lnfull; j++,k++ )); do
		SYM=${ATTACH:$j:1}
		if (( $k>=$LNS )); then
		  if [[ "$SYM" == " " ]]; then
			str=${str#'  '}
			atch=`echo -en "printf \"$fmtatch\" \"$str\" \"$LDTTM\"" | zsh`;
			outt "$atch"
			str=""; k=-1; let j--;
		  fi
		fi
		str+=$SYM
	  done
	  if [ -n "$str" ]; then
		str=${str#'  '}
		atch=`echo -en "printf \"$fmtatch\" \"$str\" \"$LDTTM\"" | zsh`;
		outt "$atch";
	  fi
	fi

	TEXTLETTER="Здравствуйте.\n\n"

	if [ -z "${WMTXTMAIL[$i]}" ]; then TEXTLETTER+="Примите сообщение от станции $STATION.\n"
	else TEXTLETTER+="${WMTXTMAIL[$i]}\n";fi
#    gettimedate
#	lnout=`echo -en "printf \"$fmtline\" \"Отправка...\" \"$LDTTM\"" | zsh`; outt "$lnout";
	updatesessionofday "$INDEX_EML"
	if [ -z "${WMATTACHM[$i]}" ]; then
	  TEXTLETTER+="\n`cat /usr/share/wlib/.signature`\n"
	  echo -e "$TEXTLETTER" | sudo mutt -s "$SUBJ" -b $BCCMAIL "${WMEMAILAD[$i]}" &> $MAILLOG &
	  sleep 1
	else
	  TEXTLETTER+="\nСмотрите $word во вложении:\n$ATTACH\n"
	  TEXTLETTER+="\n`cat /usr/share/wlib/.signature`\n"
	  echo -e "$TEXTLETTER" | sudo mutt -s "$SUBJ" -b $BCCMAIL "${WMEMAILAD[$i]}" -a ${WMATTACHM[$i]} &> $MAILLOG &
	  sleep 1
	fi
	# end line mail section
#	gettimedate
#	lnout=`echo -en "printf \"$fmtline\" \"Завершено.\" \"$LDTTM\"" | zsh`; outt "$lnout";
	if (( $i < $((im-1)) )); then outt "$lnintol"; fi
  done # i
  # end all
  lntabend="└"; lntabend+=`printf "─%.s" $(seq $lz)`;lntabend+="┘"; outt "$lntabend";
  nMAILSND=0;
}


################################################################
##
## ftprecv ftp receiver
##
## void ftprecv localdir text
##
function ftprecv()
{
  local LDIR="$1" TEXT="$2"
  local i FTPCMD="" FRECV="*";
  infoline "$TEXT"
  FTPCMD="$FTPCMDOPEN"
  FTPCMD+="cd \"$FTPSTDPATHRECV\";echo \"$FTPSTDPATHRECV\";rels;mget -O \"$LDIR\" -c \"$FRECV\";"
  FTPCMD+="$FTPCMDCLOSE"

  for (( i=1; i<=$MAXRPT; i++ )); do

	lftp -c "$FTPCMD" &> "$FTPRLOG"

	if ! [ -z "`ls $LDIR`" ]; then
	  getfsinfo "$LDIR/*"
	  TM=`date "+%H часов %M минут"` # local date
	  rita "Приняты файлы от А эС Пэ Дэ. Время $TM"
	  fslist "$FTPRLOG" "Приняты файлы от АCПД" "FTPFS"

	  if fseqsize "Приём от АСПД" "$FTPRLOG"; then
		txt="Проверьте каталог приёма."
		infoline "$txt #$i($MAXRPT)"; rita "$txt";
		rita "Размер совпадает."
		ftpdel "$FTPSTDPATHRECV/*" "SILENT"
		break;
	  else
		settextcolor $LMAGENTA
		outt "Повторный приём #$i($MAXRPT)"
		setdefcolors
		rita "Размер файла не совпадает. Повторный приём."
	  fi
	else
	  break;
	fi
  done

  if (( $i >= $MAXRPT )); then
	let i--
	infoline "Макс. кол-во $i($MAXRPT) попыток приёма. Проверьте соединение"
	rita " Максимальное количество попыток приёма."
  fi
}

################################################################
##
## ftp receiver once
##
##	chckhavefile fname rpath
##
function chckhavefile()
{
  local FNAME="$1" RMPATH="$2" LOGFL=$3;
  local ret=1;

  FTPCMD="$FTPCMDOPEN"

  if [ -z $RMPATH ]; then FTPCMD+="cd \"$FTPSTDPATHRECV\";rels;"
  else FTPCMD+="cd \"$RMPATH\";rels;"; fi

  FTPCMD+="$FTPCMDCLOSE"
  lftp -c "$FTPCMD" &> "$LOGFL"

  while read -r -u5 line; do
    FND0=`echo $line | grep "$FNAME"`
    FND1=`echo $line | grep "550"`

    if   ! [ -z "$FND0" ]; then ret=0;	break;
    elif ! [ -z "$FND1" ]; then break;
    fi
  done 5< $LOGFL

return $ret
}

################################################################
##
## ftprecvonce ftp once receiver from $FTPSTDPATHRECV
##
## int ftprecvonce rmfile lpath
##
function ftprecvonce()
{
  local RMFILE="$1" LPATH="$2"
  local ret=0; local i FTPCMD="";

  if ! [ -d "$LDIRRECV/$ARHDIR" ]; then crdsetpm "$LDIRRECV/$ARHDIR"; fi
  rfile="${RMFILE##*/}" # namefile
  rpath="${RMFILE%/*}"  # fullpath

  if chckhavefile "$rfile" "$rpath" "$FTPRLOG"; then
	for (( i=1; i <= $MAXRPT; i++ )); do
	  outt "Запрос #$i($MAXRPT)"
	  FTPCMD="$FTPCMDOPEN"

	  if ! [ -z $LPATH ]; then lpath=$LPATH
	  else lpath="$LDIRRECV/$ARHDIR"; fi

	  FTPCMD+="rels \"$rpath\";get -O \"$lpath\" -c \"$RMFILE\";"
  	  FTPCMD+="$FTPCMDCLOSE"
	  lftp -c "$FTPCMD" &> "$FTPRLOG"

	  getfsinfo "$lpath/*"

	  if fseqsize "Приём $rpath" "$FTPRLOG"; then
		outt "Файл $RMFILE принят: #$i($MAXRPT)"
		rita "Приняты файлы от А эС Пэ Дэ. Размеры файлов совпадают."
		ret=0
		break
	  else
		settextcolor $LMAGENTA
		outt "Файл $RMFILE не принят. Повтор #$i($MAXRPT)"
		rita "Файл не принят. Повтор. $i из $MAXRPT."
		setdefcolors
	  fi
	done

	if (( $i >= $MAXRPT )); then
	  let i--
	  outt "Макс. кол-во $i($MAXRPT) попыток приёма. Проверьте соединение"
	  rita "Максимальное количество попыток приёма. Проверьте соединение."
	  ret=1
    else
	  getfsinfo "$lpath/*"
	  fslist "$FTPRLOG" "Принятые" "FTPFS"
	fi
  else
	settextcolor $LMAGENTA
	outt "Файла $RMFILE на сервере нет"
	setdefcolors
  fi
return $ret
}

################################################################
##
## ftp deleter
##
## void ftpdel /rpath/fname
##
function ftpdel()
{
local FTPCMD="" RPATHFN="$1" TYPE="$2"

    rfile="${RPATHFN##*/}" # namefile
    rpath="${RPATHFN%/*}"  # fullpath

    FTPCMD="$FTPCMDOPEN"
    FTPCMD+="cd \"$rpath\";echo \"$rpath # LIST\";rels;"
    FTPCMD+="echo \"$rpath # DELETE\";mrm \"$rfile\";rels;"
    FTPCMD+="$FTPCMDCLOSE"


    if [[ "$TYPE" == "SILENT" ]]; then
        lftp -c "$FTPCMD" &> /dev/null &
    else
        lftp -c "$FTPCMD" &> "$FTPLLOG"
        fslist "$FTPLLOG" "Содержимое: $FTPSERVER$rpath" "FTPFS"
    fi
}

################################################################
##
## get fs info
##
## Get file list to put for checkers arrays (fseqsize)
## void getfsinfo pathdir
##
function getfsinfo()
{
  local i PATHDIRFL="$1" count;
  filelist=($PATHDIRFL)

  # ftp send
  count=${#filelist[@]}

  if (( $count > 0 )); then
	for (( i=0; i<count; i++ )); do
	  aFLS[$i]="${filelist[$i]##*/}" # получить имя из полного пути
	  aLSZ[$i]=`stat -f %z "${filelist[$i]}"`
	done
	ls -l "${PATHDIRFL%'*'}" > "$FLLSLOG"
  fi

  # e-mail attachments
  count=${#PTFATCH[@]}

  if (( $count > 0 )); then
	for (( j=0; j<$count; j++, i++ )); do
	  aFLS[$i]="${PTFATCH[$j]##*/}" # получить имя из полного пути
	  aLSZ[$i]=`stat -f %z "${PTFATCH[$j]}"`
	done
  fi
}

################################################################
##
## ftp/fs log translate
##
## void pathview format rpath typelist
## typelist - FLSFS, CHCKSZ, FTPFS
##
function pathview()
{
  local FORMAT="$1" RPATH="$2" TYPELIST="$3"
  local lzp tm;

  if [ "$TYPELIST" == "FTPFS" ]; then
	lzp=$(($WSLZ-32))
	export LANG=en_US.UTF-8
	export LC_ALL=en_US.UTF-8
	tm=`date "+%b %d %H:%M"` # local date
	export LANG=ru_RU.UTF-8
	export LC_ALL=ru_RU.UTF-8
	LINE=`echo -en "printf \"$FORMAT\" \"[$RPATH]:\" \" \" \"-ADR\" \"$tm\"" | zsh`
  else
	LINE=`echo -en "printf \"$FORMAT\" \"[$RPATH]:\"" | zsh`
  fi
}

function translate()
{
  local STRTRANSLATE="$1"
  CNVUTF8="`echo \"$STRTRANSLATE\" | enca -x UTF-8`"
}

################################################################
##
## ftp/fs log translate
##
## void getfnszdt str fnamesize fsizesize
##
function getfnszdt()
{
  local STR="$1" FNAMESIZE="$2" FSIZESIZE="$3"
  local glen hl FS FE;

  translate "$STR"

  FLMODE=`echo "$CNVUTF8" | awk '{ print $1 }'`

  if ! [ -z "${FLMODE##*$'/'*}" ]; then

	# fnamesize
	FNREAL=`echo "$CNVUTF8" | awk '{for (i=9;i<=NF;i++) printf "%s ",$i}'`
    FNREAL=`echo "$FNREAL" | sed 's/[[:space:]]*$//'`
	glen=${#FNREAL}	
	hl=$(($FNAMESIZE/2))
	if (( $FNAMESIZE<=$glen )); then
	  FB=`echo "$FNREAL" | cut -c1-$hl`				#begin string
	  FE=`echo "$FNREAL" | rev | cut -c1-$hl | rev`	#end string
	  TXTFNM="$FB<~>$FE"
	else
	  TXTFNM="$FNREAL"
	fi

	# sizesiize
	FSZBYTE=`echo "$CNVUTF8" | awk '{ print $5 }'`
	glen=${#FSZBYTE}
	if (( $FSIZESIZE<=$glen )); then
  	  TXTFSZ="`echo \"scale=2; $FSZBYTE/1024/1024\" | bc`M"
	else
	  TXTFSZ="$FSZBYTE"
	fi
	# user
	FUSER=`echo "$CNVUTF8" | awk '{ print $3 }'`
	# date
	FLDATE=`echo "$CNVUTF8" | awk '{for (i=6;i<=8;i++) printf "%s ",$i}'`
	FLDATE=`echo "$FLDATE" | sed 's/[[:space:]]*$//'`
  else
	TXTFNM="$CNVUTF8"; TXTFSZ=""; FSZBYTE=0; FLDATE="";
  fi
}

################################################################
##
## ErrorParce parce
##
## void ErrorParce string_for_parce
##
function ErrorParce()
{
  ErrLN=`echo "$1" | cut -d":" -f3- | cut -d" " -f4-`
}

################################################################
##
## ErrorParce parce
##
## void ErrMsg void
##
function ErrorMsg()
{
  local LINE="" lz;
  lz=$(($WSLZ-2))
  format="│$LYELLOW$BGRED%-${lz}s$TBCOLOR$BBCOLOR│"
  LINE=`echo -en "printf \"$format\" \"$ErrLN\"" | zsh`
  outt "$LINE"
}

################################################################
##
## ftp/fs log translate
##
## void fslist fslog fstitle fstype
##
## Out in UTF-8 and convert from W1251, KOI-8, CP866
##
function fslist()
{
  local FSLOG="$1" FSTITLE="$2" FSTYPE="$3";
  local i j nf=0 ret=0 format TXTFS title="" lnif lntl count ASIZE;
  local SUMSIZE=0 FCOUNT=0 LN lz lz1 lz2 lz3 usz=9 szsz=12 dtsz=14 LNN;

  ASIZE=${#aFLS[@]}

  if [ -z "$FSTITLE" ]; then title="Список";
  else title="$FSTITLE";  fi
  if [ -z $FSTYPE ]; then FSTYPE="FLSFS"; fi

  infoline "$title"

  lz=$(($WSLZ-40))
  lnif="┌"; lnif+=`printf "─%.s" $(seq $lz)`;   lnif+="┬";lnif+=`printf "─%.s" $(seq $usz)`;
  lnif+="┬";lnif+=`printf "─%.s" $(seq $szsz)`; lnif+="┬";lnif+=`printf "─%.s" $(seq $dtsz)`;
  lnif+="┐"; outt "$lnif"

  lz1=$(($WSLZ-44))
  lntl="│$BNCOLOR$BBCOLOR Имя"; lntl+=`printf " %.s" $(seq $lz1)`;
  lntl+="$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR Польз.  $TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR Размер     $TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR Дата         $TBCOLOR$BBCOLOR│"
  outt "$lntl"

  # replace
  lnif=${lnif//'┌'/'├'}; lnif=${lnif//'┬'/'┼'};	lnif=${lnif//'┐'/'┤'}; outt "$lnif";

  if [ $FSTYPE == "FLSFS" ]; then
	count=${#PTFATCH[@]}
	if (( $count > 0 )); then
	  for (( j=0; j<$count; j++ )); do
		LNN=`ls -l "${PTFATCH[$j]}"`
#		NMF="${LNN##*/}" # filename
#		PRM="${LNN%/*}"	 # pathfile
#		echo "$PRM/$NMF" >> "$FSLOG"
		echo "$LNN" >> "$FSLOG"
	  done
	fi
  fi

  while read -r -u5 LN ; do
	LINE=""
	if echo "$LN" | egrep "Ошибка|Error" > /dev/null; then ErrorParce "$LN"; continue; fi

	getfnszdt "$LN" $((lz-4)) 10

	FNAME="$TXTFNM"; FSIZE="$TXTFSZ"; FDATE="$FLDATE"; FMODE="$FLMODE";
	setdefcolors

	if ! [ -z "$FNAME" ]; then

	  if ! [ -z "${FMODE##*$'/'*}" ]; then
		let FCOUNT++
        if [ -z "${FMODE##$'d'*}" ]; then
		  FSIZE="-DIR"
		  setfilecolor $WHITE;  setsizecolor $WHITE;
		elif [ -z "${FMODE##$'l'*}" ]; then
		  FSIZE="-LNK"
		  setfilecolor $CYAN; setsizecolor $CYAN;
		else
		  let SUMSIZE+=$FSZBYTE
		  setsizecolor $RED
		fi

		if (( $ASIZE>0 )); then
		  for(( i=0; i<$ASIZE; i++ )); do
			if [ "$FNREAL" == "${aFLS[$i]}" ] && [ $FSTYPE == "FTPFS" ]; then
			  setbackcolor $BGSCOLOR; settextcolor $GRAY; setdatecolor $WHITE;
			  setsizecolor $BRED; setfilecolor $FLSCOLOR;
			  break;
			elif [ $FSTYPE == "DELAY" ]; then
			  setbackcolor $BGRED; settextcolor $LYELLOW; setdatecolor $WHITE;
			  setsizecolor $LYELLOW; setfilecolor $LYELLOW;
			  break;
			fi
		  done # for
		fi
		  format="│$FBCOLOR$BBCOLOR%-${lz}s$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR%${usz}s$TBCOLOR$BBCOLOR│$SZCOLOR$BBCOLOR%${szsz}s$TBCOLOR$BBCOLOR│$DTCOLOR$BBCOLOR%${dtsz}s$TBCOLOR$BBCOLOR│"
		  LINE=`echo -en "printf \"$format\" \"$FNAME\"  \"$FUSER\" \"$FSIZE\" \"$FDATE\"" | zsh`
		  outt "$LINE";
	  else
		fmt="│$BYELLOW$BBCOLOR%-${lz}s$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR%${usz}s$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR%${szsz}s$TBCOLOR$BBCOLOR│$DTCOLOR$BBCOLOR%${dtsz}s$TBCOLOR$BBCOLOR│"
		pathview  "$fmt" "$LN" "FTPFS"
		outt "$LINE"
		if [[ -z "${ErrLN##*$"$LN"*}" && -n $ErrLN ]]; then ErrorMsg; fi
	  fi
	fi
	let nf++
  done 5< "$FSLOG"

  setdefcolors
  # заменить символ
  lnif=${lnif:0:$lz+1}'┴'${lnif:$lz+2};  outt "$lnif";

  dt=`date "+%d %b %Y"` # local date
  lz2=$(($WSLZ-38))
  format="│Файлов: $BNCOLOR$BBCOLOR%-${lz2}s$TBCOLOR$BBCOLOR│$SZCOLOR$BBCOLOR%${szsz}s$TBCOLOR$BBCOLOR│$DTCOLOR$BBCOLOR%${dtsz}s$TBCOLOR$BBCOLOR│"
  lln=${#FSZBYTE}

  if (( $lln > 12 )); then SUMSIZE="`echo \"scale=2; $SUMSIZE/1024/1024\" | bc`M"; fi

  TXTFS=`echo -en "printf \"$format\" \"$FCOUNT\" \"$SUMSIZE\" \"$dt\"" | zsh`;  outt "$TXTFS";

  lz3=$(($WSLZ-30))
  lnif="└"; lnif+=`printf "─%.s" $(seq $lz3)`; lnif+="┴";lnif+=`printf "─%.s" $(seq $szsz)`;
  lnif+="┴";lnif+=`printf "─%.s" $(seq $dtsz)`; lnif+="┘"; outt "$lnif";

  if (( $nf==0 )); then ret=1; fi
  setdefcolors
return $ret
}

################################################################
##
## Check file size
##
## void fseqsize title
##
function fseqsize()
{
  local TITLE="$1" LOGFL="$2"
  local ret=0 j format TEXTCH lz=0 Path="" count fm szsz=12 lnif lnif2 spsz spsz2;
  setcolors $BLACK $BGGRAY $BLUE $BLACK

  count=${#aFLS[@]}

  if (( $count>0 )); then
	infoline "Размер. $TITLE"
	setbackcolor $BGGRAY; settextcolor $BLACK; setsizecolor $RED; setfilecolor $BLUE;
	lz=$(($WSLZ-32))
	lnif="┌"; lnif+=`printf "─%.s" $(seq $lz)`; lnif2="┬"; lnif2+=`printf "─%.s" $(seq $szsz)`;
	lnif+=$lnif2; lnif+=$lnif2; lnif+="┬";lnif+=`printf "─%.s" $(seq 3)`; lnif+="┐";
	outt "$lnif"
	format="│$BNCOLOR$BBCOLOR%-${lz}s$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR%-${szsz}s$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR%-${szsz}s$TBCOLOR$BBCOLOR│$BNCOLOR$BBCOLOR%-3s$TBCOLOR$BBCOLOR│"
	TEXTCH=`echo -en "printf \"$format\" \" Имя\" \" Локальный\" \" FTP АСПД\" \" S\"" | zsh`
	outt "$TEXTCH"
	# replace
	lnif=${lnif//'┌'/'├'}; lnif=${lnif//'┬'/'┼'}; lnif=${lnif//'┐'/'┤'}; outt "$lnif";

	while read -r -u5 LN ; do

	  if echo "$LN" | egrep "Ошибка|Error" > /dev/null; then ErrorParce "$LN";continue; fi

	  LINE=""
	  getfnszdt "$LN" 40 10
	  LN="$CNVUTF8"; FNAME="$TXTFNM";

      if ! [ -z "${FNAME##*$'/'*}" ]; then
		for (( j=0; j<$count; j++ )); do
		  if [ "$FNREAL" == "${aFLS[$j]}" ]; then
			FSIZE="$TXTFSZ"
			format="$FBCOLOR$BBCOLOR│%-${lz}s$TBCOLOR$BBCOLOR│$SZCOLOR$BBCOLOR%${szsz}s$TBCOLOR$BBCOLOR│$SZCOLOR$BBCOLOR%${szsz}s$TBCOLOR$BBCOLOR"
			TEXTCH=`echo -en "printf \"$format\" \"${aFLS[$j]}\" \"$FSIZE\" \"${aLSZ[$j]}\"" | zsh`
			if [[ $FSIZE == ${aLSZ[$j]} && $FSIZE > 0 && ${aLSZ[$j]} > 0 ]]; then
			  outt "$TEXTCH│$GREEN$BBCOLOR \bOK $TBCOLOR$BBCOLOR│"
			  break
			else
			  outt "$TEXTCH│$LRED$BBCOLOR \bNO!$TBCOLOR$BBCOLOR│"
			  ret=1
			fi
		  fi
		done
#		if echo "$ErrLN" | grep "$FNAME" > /dev/null; then ErrorMsg; fi
	  else
		spsz=`printf " %.s" $(seq $szsz)`; spsz2=`printf " %.s" $(seq 3)`;
		fm="│$BYELLOW$BBCOLOR%-${lz}s$TBCOLOR$BBCOLOR│$spsz│$spsz│$spsz2│"
		pathview "$fm" "$LN" "CHCKSZ"
		outt "$LINE";
		if [[ -z "${ErrLN##*$"$LN"*}" && -n $ErrLN ]]; then ErrorMsg; fi
	  fi

	done 5< "$LOGFL"
  else
	ret=1
  fi

  # replace
  lnif=${lnif//'├'/'└'}; lnif=${lnif//'┼'/'┴'};	lnif=${lnif//'┤'/'┘'};  outt "$lnif"
  setdefcolors

return $ret
}

################################################################
##
## Messages Recv
##
## void fndmngmsg msg log
##
function fndmngmsg()
{
  local ret=1 MSG="$1" FLOG="$2" fline=""

  while read fline; do

	if [[ "$fline" =~ "$MSG" ]]; then ret=0; fi

  done < $FLOG

return $ret
}

################################################################
##
## count session of day
##
## void updatesessionofday fileindex
##
function updatesessionofday()
{
  local FILEINDEX="$1"
  local NS=1
  NUMSESSOFDAY=1
  DATE=`date +%d%m` # get local date

  if [ -f "$FILEINDEX" ]; then
    SAVEDATE=`cat "$FILEINDEX" | cut -d" " -f1 | cut -d":" -f2`
    if [ "$DATE" == "$SAVEDATE" ] ; then
	  let NS=`cat "$FILEINDEX" | cut -d" " -f2 | cut  -d":" -f2`
	  let NS++
	  NUMSESSOFDAY="$NS"
	fi
  fi
  echo "Date:$DATE N:$NS Sess:$NS" > "$FILEINDEX"
}

################################################################
##
## Messages Send
##
## void msgsend lpath/lfile rpath
##

function msgsendnumber()
{
  updatesessionofday "$INDEX_SND"
  rita "Отправка на А эС Пэ Дээ номер $NUMSESSOFDAY"
  infoline "Отправка. АСПД. № $NUMSESSOFDAY"
}

function msgsend()
{
  INIT
  local FLSENDPT="$1" RMPATH="$2" TM txt txt2;

  if [ -z "$FLSENDPT" ]; then
	if ! [ -z "`ls $LDIRFORSEND`" ]; then

	  # Prepare files
	  make_METEO "avos" '}|sm`' "$INDEX_AVS"
	  make_METEO "climat" '}|klb' "$INDEX_CLM"
	  make_VOS_SVC "vos" "#!VOS" "$INDEX_VOS"
	  make_VOS_SVC "svc" "#!SVC" "$INDEX_SVC"
	  make_VRS;

	  if ! [ -z "`ls "$LDIRSEND"`" ] ; then
        msgsendnumber
  		if [ $SQUID == OFF ] && [ $BIGSIZE == ON ]; then
		  settextcolor $LGREEN; outt "Squid stop..."; setdefcolors;
		  sudo squid -k interrupt > /dev/null
		fi

		GenCmdSend
		getfsinfo "$LDIRSEND/*"
		fslist "$FLLSLOG" "Файлы на отправку:"
		emailsend
		ftpsend "$LDIRSEND"
		rm "$LDIRSEND"/*
		infoline "Отправка завершена."

	  if [ $SQUID == OFF ] && [ $BIGSIZE == ON ]; then
		settextcolor $LGREEN
		outt "Squid start..."
		setdefcolors
		sudo service squid start > /dev/null;
		BIGSIZE=OFF
	  fi
	  else
  		infoline "Файлов для отправки в $LDIRSEND нет"
  		rita "Файлов для отправки нет."
	  fi
	else
	  infoline "Файлов в каталоге: $LDIRFORSEND нет"
	  infoline "Отмена отправки"
	  rita "Отмена. Файлов для отправки нет."
	fi
  else
	msgsendnumber
	if [ -z "$RMPATH" ]; then RMPATH="$FTPSTDPATHSEND"; fi
	AddFnamePath "$FLSENDPT" "$RMPATH"
	GenCmdSend

    if [ -f "$FLSENDPT" ]; then
	  ftpsendonce "$FLSENDPT"
	else
	  settextcolor $LMAGENTA
	  outt "Файл "$FLSENDPT" не найден"
	  setdefcolors
	  rita "Файл для отправки не найден"
	fi
  fi
  status
}


################################################################
##
## E-mail Send
##
## void msgmail e-mail subj text attach
##
function msgmail()
{
  INIT
  local EMAIL="$1" SUBJ="$2" TXTMAIL="$3" ATTACH="$4"

  if [ -z "$EMAIL" ]; then
	rita "Укажите адрес."
	psecho "E-mail: "; read EMAIL;
  fi

  if [ -z "$SUBJ" ]; then
	rita "Задайте заголовок."
	psecho "Заголовок: "; read SUBJ;
  fi

  if [[ -z $ATTACH ]]; then
	if [[ -z "$TXTMAIL" ]]; then
	  rita "Наберите сообщение."
	  $EDITOR "$TMPFILE"
	  TXTMAIL=`cat "$TMPFILE"`
	  outt "TMAIL:"
	  echo -e "$TXTMAIL\n"
	  sudo rm "$TMPFILE"
	fi
  fi
  updatesessionofday "$INDEX_SND"
  AddFnamePath "$ATTACH" "$EMAIL" "$SUBJ" "$TXTMAIL"
  GenCmdSend;
  emailsend;
  status
}

################################################################
##
## Messages recv
##
## void msgrecv rpathfile lpath
##
function msgrecv()
{
  INIT
  local RMPATHFL="$1" LPATH="$2"
  local txt="";

  if [ -z "$RMPATHFL" ]; then
	ftprecv "$LDIRRECV" "Приём сообщений: $FTPSTDPATHRECV"

	if ! fndmngmsg "файлы не найдены" "$FTPRLOG" ; then

	  if ! [ -z "`ls $LDIRRECV`" ] ; then
		updatesessionofday "$INDEX_RCV"
		outt "Сохранено в .../Recv/$ARCHDIR"
		if ! [ -d  "$LDIR_ARCH/Recv/$ARCHDIR" ]; then crdsetpm "$LDIR_ARCH/Recv/$ARCHDIR"; fi
		if ! [ -d  "$RDIR_ARCH/Recv/$ARCHDIR" ]; then crdsetpm "$RDIR_ARCH/Recv/$ARCHDIR"; fi
		cp "$LDIRRECV"/* "$LDIR_ARCH/Recv/$ARCHDIR/"
		cp "$LDIRRECV"/* "$RDIR_ARCH/Recv/$ARCHDIR/"
		rm "$LDIRRECV"/*
        status
	  else
		settextcolor $GRAY
		outt "Каталог $LDIRRECV пуст"
	  fi
	else
	  txt="Нет файлов для приёма"
	  settextcolor $GRAY
	  outt "$txt";
	fi
  else
	outt "Запрос $RMPATHFL от АСПД"
	ftprecvonce "$RMPATHFL" "$LPATH"
	updatesessionofday "$INDEX_RCV"
	status
  fi
}

################################################################
##
## lsallpaths
##
## void lsallpaths void
##
function lsallpaths()
{
  local i;

  echo "${WFTPPTH[0]}" > "$TMP"
  for (( i=1; i<${#WFTPPTH[@]}; i++ ));do
	# get all ftp path exclude e-mail
	echo "${WFTPPTH[$i]}" | grep -v '@' >> "$TMP"
  done

  for ftppath in `sort -u "$TMP"`; do
    FTPCMD+="echo \"$ftppath\";rels \"$ftppath\";"
  done
}

################################################################
##
## Messages Check
##
## void msgrl rpath
##
function msgrl()
{
  INIT
  local FTPPATH="$1";
  local FTPCMD="$FTPCMDOPEN" FTPLS="";

  if [ -z "$FTPPATH" ]; then
	FTPCMD+="cd \"$FTPSTDPATHSEND\";echo \"$FTPSTDPATHSEND\";rels;cd \"$FTPSTDPATHRECV\";echo \"$FTPSTDPATHRECV\";rels;"
  elif [ "$FTPPATH" == "-a" ];then
	lsallpaths
  else
	FTPCMD+="cd \"$FTPPATH\";echo \"$FTPPATH\";rels;"
  fi

  FTPCMD+="$FTPCMDCLOSE"
  infoline "Просмотр каталогов сервера АСПД ..."

  lftp -c "$FTPCMD" &> "$FTPLLOG"

  if ! fndmngmsg "Ошибка доступа: 550" "$FTPLLOG" ; then
	fslist "$FTPLLOG" "Сервер $FTPSERVER" "FTPFS"
  else
	settextcolor $LMAGENTA
	outt "Каталог $FTPPATH не существует или не доступен"
	setdefcolors
  fi
  status
}

AWRLT=""

function genawr()
{
    while read -r -u5 awrline ; do
        if (( $(strlen "$awrline") > 1 )); then
            if [ -z "${awrline##$"UTC:"}" ]; then
                AWRLT+="!   UTC $(date -u "+%H:%M %d.%m.%Y")\n"
            elif [ -z "${awrline##$"@"*}" ] || [ -z "${awrline##$"!"*}" ]; then            
                AWRLT+="$awrline\n"
            else
                AWRLT+="  $awrline\n"
            fi
        else 
            AWRLT+="\n"
        fi
    done 5< "$TEMPLATES/lt_fap.templ"
}

################################################################
##
## Messages Generate
##
## void msglt typeletter 
##
## To upper symbols ${var^^} lower ${var,,}
##
function msglt()
{
  local TYPELETTER="$1" GRPMSEND="$2" SECONDTYPE="$3" DT GRP=false TM TLG SIGNATURE txt nSZ sz CNSL="NO" lenT

  declare -a NMGRP

  setcolors $GRAY $BGBLUE
  TM=`date "+%H часов %M минут"`
  txt="Создание сообщения"; infoline "$txt"; rita "$txt. Время $TM";

  # Выбор типа сообщения
  if [ -z "$TYPELETTER" ]; then
	outt " avos - метео сводка         climat - климатическая метеосводка"
	outt "  vos - телеграмма              svc - служебная телеграмма"
	outt "  vrs - архивная телеграмма     grp - отправка на группу"
	outt "  awr - авиа погода          e(xit) - отмена"
	str1="Выберите тип сообщения:"
	str2="(avos|climat|awr|vos|svc|vrs|grp|e(xit):"
	rita "$str1"

	while true; do
	  outt "$str1";
	  psecho "$str2";
	  read lttype;

	  if [ $lttype == "e" ]; then outt "Отмена"; return;
	  elif [ "$lttype" == "grp" ] || [ "$lttype" == "vos" ]\
        || [ "$lttype" == "svc" ] || [ "$lttype" == "vrs" ]\
		|| [ "$lttype" == "avos" ] || [ "$lttype" == "climat" ]\
        || [ "$lttype" == "awr" ]; then
		break;
	  else
		outt "Неизвестный тип сообщения $lttype"
		outt "Отмена"
		help letter
		return
	  fi
	done
  else
	lttype="$TYPELETTER"
	if [ $lttype == "e" ]; then outt "Отмена"; return;
	elif [ "$lttype" == "grp" ] || [ "$lttype" == "vos" ]\
      || [ "$lttype" == "svc" ] || [ "$lttype" == "vrs" ]\
	  || [ "$lttype" == "avos" ] || [ "$lttype" == "climat" ]\
      || [ "$lttype" == "awr" ]; then
	  pass=1
	else
	  outt "Неизвестный тип сообщения $lttype"
	  outt "Отмена"
	  help letter
	  return
	fi
  fi # choice type messages end
  # choice adresses

  if [[ "$lttype" == "grp" ]]; then
  	 GRP=true
     if [ -z $GRPMSEND ]; then
        lttype="vos"
        abook
	    txt="Групповая рассылка"
	    outt "$txt"; rita "$txt"
	    rita "Введите имена групп, через пробел."
	    str="Введите имена групп (имя1 имя2 ... e(xit):"
	    while true; do
	        psecho "$str"; read ngroups; ngroups=${ngroups^^};
	        if [[ "$ngroups" =~ ^([eE])+$ ]]; then outt "Отмена."; return;
	        else
		        i=0
		        for sgrp in $ngroups; do
		            NMGRP[$i]="$sgrp"
	  	            let i++
		        done
		        break;
	        fi
	    done
     else
        NMGRP[0]="$GRPMSEND"
        if ! [ -z $SECONDTYPE ]; then
            lttype=$SECONDTYPE
        else 
            lttype="vos"
        fi
    fi
  fi  
    
  if [ "$lttype" == "avos" ]; then
	txt="Отправка СИН сообщения"
	infoline "$txt"; rita "$txt"
  elif [ "$lttype" == "climat" ]; then
	txt="Отправка сообщения о климате"
	infoline "$txt"; rita "$txt"
  elif ! $GRP; then
    abook
    if [ "$lttype" == "awr" ]; then
	    txt="Отправка сообщения ФАП/AWR"
	    infoline "$txt"; rita "$txt"
    else
        outt "Тип сообщения $lttype"
    fi
	rita "Введите порядковые номера адресов, через пробел."
	str="Выберите адреса (1 2 3... e(xit):"
	while true; do
	  psecho "$str"; read adress;
	  if [[ "$adress" =~ ^([eE])+$ ]]; then	outt "Отмена.";	return;
	  else
		i=0
		for sadr in $adress; do
		  adr[$i]=$sadr
	  	  let i++
		done
		break;
	  fi
	done
  fi   # choice adresses end

  # add info to letter

  if [ "$lttype" == "avos" ]; then
	FNAME="avos.txt"
	TMPFILE=$TMPASPD/$FNAME
	crfsetpm "$TMPFILE"
	SIGNATURE="}|sm\` $METEOSIGNATURE "
	echo -en "$SIGNATURE" > "$TMPFILE"
  elif [ "$lttype" == "climat" ]; then
	FNAME="avosclimat.txt"
	TMPFILE=$TMPASPD/$FNAME
	crfsetpm "$TMPFILE"
	SIGNATURE="}|klb $METEOSIGNATURE 111 "
	echo -en "$SIGNATURE" > "$TMPFILE"
  elif [[ "$lttype" == "vos" || "$lttype" == "svc" || "$lttype" == "vrs"  || "$lttype" == "awr" ]]; then
	if [[ "$lttype" == "vos" || "$lttype" == "svc" || "$lttype" == "vrs"  ]]; then
	    FNAME="$lttype.txt"
	    TMPFILE=$TMPASPD/$FNAME
	    crfsetpm "$TMPFILE"
	    echo -en "#!${lttype^^} RADIO !\n" > "$TMPFILE"
    elif [ "$lttype" == "awr" ]; then
        FNAME="vos.txt"
	    TMPFILE=$TMPASPD/$FNAME
	    crfsetpm "$TMPFILE"
	    echo -en "#!AWR RADIO !\n" > "$TMPFILE"
	fi
	format="%-s"; local TEXTFMT="";

	if $GRP; then
	  OLDIFS="$IFS"; IFS=';'
	  for NM in ${NMGRP[@]}; do
		for vadr in ${WADRGROUP[$NM]}; do
          if [ "$lttype" == "awr" ]; then  
		    echo -en "@$vadr;SUBJ:VOSTOK St.-ФАП/AWR-$(date -u "+%H:%M %d.%m.%Y") UTC\n" >> "$TMPFILE"
          else
            echo -en "@$vadr\n" >> "$TMPFILE"
          fi  
		done
	  done
	  IFS="$OLDIFS"
	else
	  for (( i=0; i<$COUNTADRESS; i++ )); do
		for (( j=0; j<${#adr[@]}; j++ )); do
		  if (( "$(($i+1))"=="${adr[$j]}" )); then
			TEXTFMT=`echo -en "printf \"$format\" \"@${WADRESS[$i]}\"" | zsh`
			echo "$TEXTFMT" >> "$TMPFILE"
			outt "Отправка для ${WADRESS[$i]}"
		  fi
		done
	  done
	fi

    if [ "$lttype" == "awr" ]; then
        genawr
        echo -en "$AWRLT\n" >> "$TMPFILE"    
    else
	    echo "" >> "$TMPFILE"
	    echo "#ТЕКСТ" >> "$TMPFILE"
	    echo "" >> "$TMPFILE"
	    echo "#CHEEF:" >> "$TMPFILE"
    fi
  else
	outt "Неизвестный тип сообщения $lttype. Отмена"
	help letter
	return
  fi

  # Завершение формирования письма
  WL_L1=true
  while $WL_L1; do

    if [[ $lttype == avos || $lttype == climat ]]; then
        WL_L2=true
	    while $WL_L2; do
                
            lnm=`printf "─%.s" $(seq $WSLZ)`; 
	        scale=`printf " %.s" $(seq 9)`; scale+="1"
	        scaler="$FLSCOLOR$BGSCOLOR \bx10b$GRAY$BBCOLOR"
            for (( i=2; i<$(($WSLZ/10)); i++)); do scale+="${scale:0:9}$i"; done
            scale+=${scale:0:6}"$scaler"
	        VL="$FLSCOLOR$BGSCOLOR$MINFSIZE$GRAY$BBCOLOR"
	        MRU="$LYELLOW$BBCOLOR▼$GRAY$BBCOLOR"
            scale=${scale:0:$MINFSIZE-1}"$VL"${scale:$MINFSIZE+1}
            lnm1=""
            for (( i=0; i<$(($WSLZ/5)); i++)); do lnm1+="${lnm:0:4}┼"; done
	        lnm1=${lnm1:0:$MINFSIZE-1}"$MRU"${lnm1:$MINFSIZE}

            outt -s "$lnm"; 
            settextcolor "$LGREEN"
            outt -s "Введите данные. Ввод данных завершить символом '='. Сохранить [Enter]"
            setdefcolors
	        outt -s "$lnm"
	        outt -s "$scale"; outt -s "$lnm1" 
            #------------------------------------------       
	        echo -en "$LCYAN"; read -p "$SIGNATURE" -e TLG

	        if [ $lttype == avos ]; then outt -l "}|sm"'\`' "$METEOSIGNATURE $TLG"
	        else outt -l "$SIGNATURE $TLG";	fi

            lnm2=${lnm1//'┼'/'┴'};  lnm2=${lnm2//'▼'/'▲'}; 
	        outt -s "$lnm2" 
            #------------------------------------------
	        setdefcolors; att=''
	        lenT=$(strlen "$TLG"); LASTLITER=${TLG:lenT-1:1};
	        if [[ $LASTLITER != "=" ]]; then TLG+='='; att=" (добавлено '=')"; fi

	        nSZ="$(strlen "$SIGNATURE$TLG")"; sz=${#nSZ}
	        ffw=13; lz=$(($WSLZ-$ffw-$sz-1));
	        format="$TBCOLOR$BBCOLOR%-${ffw}s$SZSCOLOR$BBCOLOR %-${sz}s$TBCOLOR$BBCOLOR%-${lz}s"
	        tsz=`echo -en "printf \"$format\" \"Размер файла:\" \"$nSZ\" \"b$att\"" | zsh`
	        outt -s "$tsz";

            if (( $MINFSIZE > $nSZ ));then 
		        settextcolor "$LYELLOW"; setbackcolor $BGRED
		        outt -s "Размер сообщения меньше минимального. Повторите ввод."
		        outt -l "Повторный ввод"
		        setdefcolors
		        continue
	        fi	  
	        outt -l "$tsz";

            while true; do  
	            psecho "Введено верно?(y/n/a):"; read sn;
	            case $sn in
	            y|Y) 	
			        outt -s "Данные для отправки:"
			        outt -s "$lnm" 
                    #------------------------------------------
                    settextcolor "$LCYAN"
			        if [ $lttype == avos ]; then outt -s "}|sm"'\`' "$METEOSIGNATURE $TLG"
			        else outt -s "$SIGNATURE $TLG";	fi
                    setdefcolors
                    #------------------------------------------
			        outt -s "$lnm" 
		            echo -en "$TLG" >> "$TMPFILE"
		            sudo chown $USER:aspd "$TMPFILE"
                    WL_L2=false
		            break; 
	            ;;
	            a|A) 
                    if [ -f "$TMPFILE" ]; then rm "$TMPFILE"; fi
		            outt "Отмена."  
		            return
	            ;;
                n|N)
                    break
                ;;
	            *)
                ;;
	            esac
            done
	    done # WL_L2
    else
	    $EDITOR "$TMPFILE" ## Editor set in internal variable $EDITOR
    fi

    while true; do
        str="Отправить сообщение?(y/n/редактировать(e)):"
	    psecho "$str"; read ch;

	    case $ch in
	    y|Y) mv "$TMPFILE" "$LDIRFORSEND/"
		    txt="Сообщение отправляется."
		    outt "$txt"; rita "$txt";
		    msgsend
            WL_L1=false
		    break
	    ;;
	    n|N) txt="Отправка отменена."
		    outt "$txt"; rita "$txt.";
		    while true; do
		        psecho "Сохранить сообщение?(y/n):"; read ch1;
		        case $ch1 in
		        n|N)
		  	        rm "$TMPFILE"
		  	        txt="Сообщение удалено."
		  	        outt "$txt"; rita "$txt";
		  	        break
		        ;;
		        y|Y)
			        DT=`date +%m%d-%H%M` # local date
			        mv "$TMPFILE" "$UNKNOWNDIR/$DT-$FNAME"
			        txt="Сообщение сохраняется в каталоге для неотправленных:"
			        outt "$txt"; rita "$txt";
			        outt "$UNKNOWNDIR/$DT-$FNAME"; 
			        break
                ;;
		        *)
		        ;;
		        esac
		    done
            WL_L1=false
		    break
        ;;
        e|E)
            break
	    ;;
	    *)
	    ;;
	    esac
    done
  done
  if [ -f "$TMPFILE" ]; then rm "$TMPFILE"; fi
}

################################################################
##
## Messages List
##
## void msgll lpath
##
function msgll()
{
local LPATH="$1" VIEW="$1";

  if [ -z "$LPATH" ]; then VIEW=all; fi

  if echo "$LPATH" | grep -v "/" > /dev/null; then

	if [[ $VIEW == all ]]; then infoline "Каталоги отправки / приёма"; fi
	
	if [[ $VIEW == all || $VIEW == meteo ]]; then
	  txt="Метео"
	  if [ -d "$LDIR_ARCH/Meteo/$ARCHDIR" ] ; then
		getfsinfo "$LDIR_ARCH/Meteo/$ARCHDIR/*"
		fslist "$FLLSLOG" "$txt"
	  else
		outt "$txt: Не было отправки СИН"
	  fi
	fi

	if [[ $VIEW == all || $VIEW == delay ]]; then
	  txt="Отложенный СИН"
	  if [ -f "$LDIRDELAY/avos.txt" ]; then
		getfsinfo "$LDIRDELAY/*"
		fslist "$FLLSLOG" "$txt" "DELAY"
	  else
		outt "$txt: Нет отложенных СИН"
	  fi
	fi

	if [[ $VIEW == all || $VIEW == send ]]; then
	  txt="Отправленные"
	  if [ -d "$LDIR_ARCH/Send/$ARCHDIR" ] ; then
		getfsinfo "$LDIR_ARCH/Send/$ARCHDIR/*"
		fslist "$FLLSLOG" "$txt"
	  else
		outt "$txt: Не было отправок"
	  fi
	fi

	if [[ $VIEW == all || $VIEW == recv ]]; then
	  txt="Принятые"
	  if [ -d "$LDIR_ARCH/Recv/$ARCHDIR" ] ; then
		getfsinfo "$LDIR_ARCH/Recv/$ARCHDIR/*"
		fslist "$FLLSLOG" "$txt"
	  else
		outt "$txt: Не было приёма"
	  fi
	fi
	else
	  getfsinfo "$LPATH/*"
	  fslist "$FLLSLOG" "$LPATH"
	fi

  if [[ $VIEW == all ]]; then status; fi
}

################################################################
##
## Messages Delete
##
## void msgdel rpath/rfile
##
function msgdel()
{
  INIT
  local RMPATH="$1"

  if [ -z "$RMPATH" ];then
	rpathfile="$FTPSTDPATHRECV/*"
  else
	rpathfile="$RMPATH"
  fi
  outt "Удаление $rpathfile"
  ftpdel "$rpathfile"
  updatesessionofday "$INDEX_DEL"
  status
}

################################################################
##
## Status out stat info about send/recv
##
## void status void
##
function status()
{
  settextcolor $BLACK; setfilecolor $RED; setbackcolor $BGGRAY;
  local i sz=2 TMELAPS LINE="" opt="$1"
  local CSIGNAVS CSIGNVOS CSIGNSVC CSIGNVRS CSIGNSND CSIGNRCV CSIGNDEL CSIGNEML;
  local NP=$TBCOLOR$BBCOLOR VP=$FBCOLOR$BBCOLOR SC=$BLUE$BBCOLOR RC=$RED$BBCOLOR DC=$BLACK$BBCOLOR EC=$GREEN$BBCOLOR

  getsign "$INDEX_AVS"; CSIGNAVS="$GSIGN"; getsign "$INDEX_VOS"; CSIGNVOS="$GSIGN";
  getsign "$INDEX_SVC"; CSIGNSVC="$GSIGN"; getsign "$INDEX_VRS"; CSIGNVRS="$GSIGN";
  getsign "$INDEX_SND"; CSIGNSND="$GSIGN"; getsign "$INDEX_RCV"; CSIGNRCV="$GSIGN";
  getsign "$INDEX_DEL"; CSIGNDEL="$GSIGN"; getsign "$INDEX_EML"; CSIGNEML="$GSIGN";

#  CSIGNSND="99"; CSIGNRCV="99"; CSIGNDEL="99"; CSIGNEML="99";

  format="$NP \bS$SC%-2s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNSND\"" | zsh`;
  format="$NP \bR$RC%-2s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNRCV\"" | zsh`;
  format="$NP \bD$DC%-2s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNDEL\"" | zsh`;
  format="$NP \bM$EC%-2s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNEML\"" | zsh`;

  format="$NP░AVS$VP%4s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNAVS\"" | zsh`;
  format="$NP VOS$VP%6s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNVOS\"" | zsh`;
  format="$NP SVC$VP%6s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNSVC\"" | zsh`;
  format="$NP VRS$VP%6s";	LINE+=`echo -en "printf \"$format\" \"$CSIGNVRS\"" | zsh`; LINE+="$NP";

  timer; TMELAPS="$VP$TIMERVAL$NP"
  local shft=76; lns=${#TIMERVAL}

  if [ $WORKTYPE == debug ]; then shft=78; else TMELAPS+="░"; fi

  lz=$(($WSLZ-($shft+$lns))); LINE+="░$VERSION"

  format="$NP░%.s";	 LINE+=`printf "$format" $(seq $lz)`;
  format="%${lz}s";  LINE=`echo -en "printf \"$format\" \"$LINE$TMELAPS\"" | zsh`

  infoline $opt "$LINE";
  echo "$TIMERVAL" > $STASAVE
}

################################################################
##
## CLOSE
##
## void CLOSE typelocked
##
function locked()
{
  local TYPELOCKED="$1"
  ret=1
  if [ -f "$TYPELOCKED" ]; then
#	  settextcolor $LRED
#	if [ "$TYPELOCKED" == "$LOCKED_SEND" ]; then
#	  outt "Operation SEND now passed"
#	elif [ "$TYPELOCKED" == "$LOCKED_RECV" ]; then
#	  outt "Operation RECV now passed"
#	elif [ "$TYPELOCKED" == "$LOCKED_MAIL" ]; then
#	  outt "Operation MAIL now passed"
#	fi
#	setdefcolors
#	usage
	ret=0
  else
	echo "$TYPELOCKED" > "$TYPELOCKED"
  fi
return $ret
}

#############################################################
##
## INIT begin seccion aspd
##
## void INIT void
##
function INIT()
{
  getadrbook;
  permInfodir
  FTPCMDOPEN="open $FTPSERVER;user $FTPUSER $FTPPASSWD;"
  FTPCMDCLOSE="bye;"
  timer;
}

################################################################
##
## HELP
##
## usage out screen help
##
## void usage typehelp
##
function unknown()
{
  local TYPEHELP="$1"
  local i;
  if ! [ -z "$TYPEHELP" ]; then
	echo -e " Команда $WHITE$TYPEHELP$STDCL не имплементирована."
  fi
  echo -e " Справка:$WHITE aspd help $STDCL\n"
}

  HLPCMD[0]="send|se|s [/lpath/file] [rpath]"
  HLPTXT[0]="Oтправка сообщений для АСПД"
  HLPCMD[1]="recv|re|r [/rpath/file] [lpath]"
  HLPTXT[1]="Приём сообщений от АСПД."
  HLPCMD[2]="del|dl|d [/rpath/rfile]"
  HLPTXT[2]="Удаление файлов из каталогов сервера АСПД
\t\t\t\t   ($LREDБудьте осторожны! Не удалите чужие файлы!$STDCL)"
  HLPCMD[3]="email|em|m e-mail [-s [subj]] [-t [text]] [-a [attach]] [-f [filemail(txt)]]"
  HLPTXT[3]="\n\tОтправка e-mail. Ещё... aspd help email"
  HLPCMD[4]="letter|lt|e [avos|climat|awr|vos|svc|vrs|grp(vos)]\n\tletter|lt|e [grp [namegrp]]"
  HLPTXT[4]="Создание письма. Выберите тип и адрес.
\tПри выборе$CYAN grp$STDCL формируется письмо типа$CYAN vos$STDCL по группе адресной книги"
  HLPCMD[5]="setperm|sp|p [-v]"
  HLPTXT[5]="Установка прав на рабочие каталоги.\n\t$WHITE-v$STDCL - подробно"
  HLPCMD[6]="rlist|rl|i [-a][rpath]";
  HLPTXT[6]="Проверка состояния каталогов удалённого сервера.\n\t$WHITE-a$STDCL - Показать содержимое всех каталогов по всей адресной книге.
\tEсли [rpath] не задан выдаётся содержимое каталогов, определённых в конфигурационном файле$CYAN aspd.conf$STDCL
\tКаталоги определяются по параметрам $CYAN \babook.d$STDCL и $CYAN \baspd.conf$STDCL автоматически"
  HLPCMD[7]="llist|ll|l [/lpath|meteo|send|recv|delay]";
  HLPTXT[7]="Cписок файлов по локальным каталогам отправки/приёма\n\
\tЕсли [/lpath/[file]] не задан выдаётся содержимое каталогов хранения, определённых в конфигурационном файле$CYAN aspd.conf$STDCL
\tили
\t - meteo - метео отправки
\t - send  - другие отправленные
\t - recv  - принятые
\t - delay - отложенный СИН
\tКаталоги определяются по параметрам$CYAN aspd.conf$STDCL и создаются автоматически"
  HLPCMD[8]="rita|rt|t [текст]";
  HLPTXT[8]="Голосовое сообщение."

  HLPCMD[9]="abook|ab|a"
  HLPTXT[9]="Вывод адресов отправки. Определяется в$CYAN abook.d$STDCL"
  HLPCMD[10]="stat|st|u"
  HLPTXT[10]="Вывод статистики"
  HLPCMD[11]="ver|v"
  HLPTXT[11]="Вывод версии"
  HLPCMD[12]="rmdl"
  HLPTXT[12]="Удалить отложенные отправки"
  HLPCMD[13]="getcfg|gf|g"
  HLPTXT[13]="Получить значение конфигурационного праметра для сторонней программы"

  HLPCMD[14]="help|h [command]";
  HLPTXT[14]="Эта справка"

  HLPNOTE[0]="$LYELLOW \bFiles:$WHITE aspd$STDCL\t\t - главный рабочий файл"
  HLPNOTE[1]="$WHITE\taspd_inc.sh$STDCL\t - включаемый файл основных функций"
  HLPNOTE[2]="$WHITE\ttcolor_inc.sh$STDCL\t - включаемый файл цветовых определений и функций"
  HLPNOTE[3]="$WHITE\tsysfns_inc.sh$STDCL\t - включаемый файл системных функций"
  HLPNOTE[4]="$WHITE\taspd.conf$STDCL\t - общий конфигурационный файл"
  HLPNOTE[5]="$WHITE\tabook.d$STDCL\t\t - адресная книга"
  HLPNOTE[6]="$LYELLOW \bNote:$STDCL\t1. Если параметры комманд не указаны, то приём/отправка/выполнение"
  HLPNOTE[7]="\tидут по параметрам определённым в$CYAN aspd.conf$STDCL"
  HLPNOTE[8]="\t2. root: Установить в$CYAN sudoers (sudo)$STDCL:\n$CYAN\tCmnd_Alias CMD=/usr/bin/aspd,/usr/local/bin/mcedit,/usr/local/bin/mcview,\ \n\t/bin/chmod,/usr/bin/touch,/bin/mkdir,/bin/cp,/bin/mv,/bin/rm,/usr/sbin/chown,service,squid\n\t%aspd ALL=NOPASSWD: CMD $STDCL"
  HLPNOTE[9]="\t3. Requaired: $CYAN$Requaired$STDCL"

function help()
{
  local typehlp="$1" TXT TB="  " ih;
  format="$TB \b$WHITE \baspd %-s$STDCL - %s\n"
  fmt="%-s\n\n"

  case $typehlp in
  send|se|s)   ih=0; TXT="Ex.:\taspd send\n\taspd s ./src/fileto.send /rae/bel"; ;;
  recv|re|r)   ih=1; TXT="Ex.:\taspd recv\n\taspd r  /rae/vos/file4.recv ./src/"; ;;
  del|dl|d)	   ih=2; TXT="Ex.:\taspd del /rae/vos/fileto.del"; ;;
  email|em|m)  ih=3;
	echo -en "printf \"$format\" \"${HLPCMD[$ih]}\" \"${HLPTXT[$ih]}\"" | zsh
	TXT="Ex.:\taspd email vostok.station@yandex.ru -a \"./test_vos\" -t \"Текст письма\" -s \"Заголовок письма\"\n"
	echo -e "$TB$TXT";	return; ;;
  letter|lt|e)  ih=4; TXT="Ex.:\taspd letter\n\taspd lt vos\n\taspd lt avos"; ;;
  setperm|sp|p) ih=5; TXT="Ex.:\taspd setperm\n\taspd sp -v"; ;;
  rlist|rl|i)   ih=6; TXT="Ex.:\taspd rlist\n\taspd rl -a\n\taspd rl /rae/vos"; ;;
  llist|ll|l)   ih=7; TXT="Ex.:\taspd llist\n\taspd ll ./src"; ;;
  rita|rt|t)    ih=8;
	echo -en "printf \"$format\" \"${HLPCMD[$ih]}\" \"${HLPTXT[$ih]}\"" | zsh
	TXT="Ex.:\taspd rita \"Привет мир\"\n";
	echo -e "$TB$TXT"; return; ;;
  abook|ab|a)   ih=9; TXT="Ex.:\taspd abook\n\taspd ab\n\taspd a"; ;;
  stat|st|u)    ih=10; TXT="Ex.:\taspd status"; ;;
  ver|v)	    ih=11; TXT="Ex.:\taspd ver"; ;;
  rmdl)	    	ih=12; TXT="Ex.:\taspd rmdl"; ;;
  getcfg|gf|g)	ih=13; TXT="Ex.:\taspd getcfg \"NAMECFGPAR\""; ;;
  help|h)	    ih=14; TXT="Ex.:\taspd help rita"; ;;
  *) unknown "$typehlp"; return; ;;
  esac

  echo -en "printf \"$format\" \"${HLPCMD[$ih]}\" \"${HLPTXT[$ih]}\"" | zsh
  echo -en "printf \"$fmt\" \"$TB \b$TXT\"" | zsh
}

function usage()
{
  local i LINE="" format;

  echo -en "\n$WHITE aspd$STDCL Отправка и приём сообщений АСПД\n"
  echo -en " usage:$WHITE aspd [command]$STDCL\n\n"
  echo -en " Commands:\n"

  format="$WHITE %-31s$STDCL - %s\n"
  for (( i=0; i<${#HLPCMD[@]}; i++ )); do
	echo -en "printf \"$format\" \"${HLPCMD[$i]}\" \"${HLPTXT[$i]}\"" | zsh
  done
  echo

  format=" %s\n"
  for (( i=0; i<${#HLPNOTE[@]}; i++ )); do
	echo -en "printf \"$format\" \"${HLPNOTE[$i]}\"" | zsh
  done
  echo
  version
}

################################################################
##
## getcfg
##
## str getcfg parameter
##
function getcfg()
{
  echo -en "${$1}"
}

################################################################
##
## CLOSE
##
## void CLOSE void
##
function CLOSE()
{
  if [ -f "$LOCKED_SEND" ]; then rm -f "$LOCKED_SEND" &> /dev/null; fi
  if [ -f "$LOCKED_RECV" ]; then rm -f "$LOCKED_RECV" &> /dev/null; fi
  if [ -f "$LOCKED_MAIL" ]; then rm -f "$LOCKED_MAIL" &> /dev/null; fi
  if [ -f "$TMP" ]; then rm -f "$TMP" &> /dev/null; fi
  if [ -f "$TMPFILE" ]; then rm -f "$TMPFILE" &> /dev/null; fi
  if [ -d "$TMPSTORE" ]; then rm -f "$TMPSTORE"/* &> /dev/null; fi
}
