##############################################################################
##
## libbash part - services function
##
## wocson (c) 2008 BSD
##
##############################################################################
NAMEPKG="libbash"
VERPKG="0.9.01"

. $HOME/.local/share/shells/lib/tcolors.sh

ARGV[0]="$0"; let i=1; for arg in "$@"; do ARGV[$i]="$arg"; let i++; done
ARGC=${#ARGV[@]}

TL=$(tput lines); TC=$(tput cols);

gX=0; gY=0;

DialogType="text" # window/text
UCH="255"
DIALOG=${DIALOG=dialog}

function gotoxy()
{
    echo -en "\033[$2;$1"; echo -en 'f'
}

function setcolor()
{
    echo -en "$1"
}

function setbackcolor()
{
    echo -en $1
}

function clrscr()
{
    echo -en $CLSTD
    clear
}

function outtextxy()
{
    gotoxy $1 $2
    echo -en "$3"
}

function _outtextxy()
{
    gotoxy $1 $2 ;
    [ -z "$4" ] && format="%s" || format="$4";
    echo "printf \"$format\" \"$3\"" | zsh
}

function strlen()
{
    local string="$1"
    echo -en "${#string}"
}

function getxy()
{
    local pos
    IFS='[;' read -p $'\e[6n' -d R -a pos -rs || echo "failed with error: $? ; ${pos[*]}"
    gX="${pos[2]}"; gY="${pos[1]}";
}

function getcursorxy()
{
  local pos
  IFS='[;' read -p $'\e[6n' -d R -a pos -rs || echo "failed with error: $? ; ${pos[*]}"
  gX=${pos[2]}; gY=${pos[1]};
}

Outtext() 
{ 
    local sh="";                  
    OLDIFS="$IFS"
    IFS="\\"
    for arg in $@; do echo -en "$sh$arg"; sh=""; done; 
    IFS="$OLDIFS"
}

TRACE()
{ 
    if [[ $DEBUG == ON ]]; then
        local sh=":"; echo -en "\n"; echo -en "${RED}TRACE: ${STDCL}" 
        for arg in $@; do echo -en "$arg$sh "; sh=""; done 
        echo -en "\n"
    fi
    echo -en "\n"
}

wprint() 
{ 
    LASTTXT=""
    printf -v LASTTXT "$@" 
    printf "${LASTTXT}${STDCL}\r"
}

Error() { echo -en "${ERR} ERROR: "; Outtext $@; echo -en "${STDCL}\n"; }

Randomz()
{
    # "abcdefghijklmnopqrstuvwxyz1234567890"
    local chars="$1"
    for i in {1..3} ; do
        echo -n "${chars:RANDOM%${#chars}:1}"
    done
    echo
}

cpdir()
{
    local SRCDIR="$1" DSTDIR="$2" 
    [[ -d "$DSTDIR" ]] && cp -rf "$SRCDIR"/* "$DSTDIR"/ || cp -rf "$SRCDIR" "$DSTDIR"
}

Choice()
{
    local answ="" Question="$1" defansw="$2" 
    read -p " $Question [$defansw]: " answ; answ=${answ:-$defansw}
    echo "$answ"
}

YesNo()
{
    local Title="$1" Question="$2" 
    UCH="255"

    if [[ "$DialogType" == "window" ]]; then
        $DIALOG --clear \
        --title " $Title " \
        --yesno "${Question}" 5 50
        case $? in
        0) UCH="0";;
        1) UCH="1";;
        255) UCH="255";;
        *)UCH="255";;
        esac
    else
        case $(Choice "$Question" "Yes/No") in
        y|Y) UCH="0";;
        n|N) UCH="1";;
          *) UCH="255";;
        esac
    fi
}

Gline()
{ 
    [[ -z "$2" ]] && syml="─" || syml="$2";
    echo -en $1 ; wprint "${syml}%.0s" $(seq $TC) ; echo "" 
}

#
# boxInfo "string1" "string2" "looooooooooooooooooooooooooongstring"
#
TITLEBOX=""
TLCLR="${BLUE}${BGWHITE}"
FGCLR="${BLACK}"
BGCLR="${BGWHITE}"
boxInfo() 
{
    local msglines="${*}"
    local -i mxlen=0

    for str in $msglines; do (( "${#str}+2" >= "$mxlen" )) && let mxlen=${#str}+2; done
    let mxlens=$mxlen-2
    [[ -n "$TITLEBOX" ]] && printf "${TLCLR} %-${mxlen}s ${STDCL}\n" "$TITLEBOX" 
    printf "${FGCLR}${BGCLR}┌"; printf "─%.0s" $(seq $mxlen); printf "┐${STDCL}\n";
    for str in $msglines; do printf "${FGCLR}${BGCLR}│ %-${mxlens}s │${STDCL}\n" "$str"; done
    printf "${FGCLR}${BGCLR}└"; printf "─%.0s" $(seq $mxlen); printf "┘${STDCL}\n"
}

declare -A HelpCnt
declare -a HelpVarCmd
let hIndex=0
NAMEPGR="unnamed (unset)"
VERPRG="0.0.XX (unset)"
Annotation="(unset)"
CommonFormat="(unset)"
let iN=0

getVerprg()
{
    echo -en "$VERPRG"
}

HelpAnnotation()
{
    [[ "$Annotation" == "(unset)" && -n "$1" ]] && Annotation="$1"
}

HelpVarCmd()
{
    CommonFormat[$hIndex]="$1"
    let hIndex=+1
}

HelpSubCmd()
{
    local key="$1" description="$2"

    if [[ "$(isakey $key)" == "NO" ]]; then
        HelpCnt["$key"]="$description" 
        let iN+=1
    fi
}

Helpline()
{
    let szN="$1+2" szC="$2+1"
    local cmd="$3"

    if [[ $(isakey $cmd) == "NO" ]]; then
        format="%-${szN}s${BRED}%-${szC}s${STDCL} - %-s"
        wprint "\n$format" " " "${cmd}" "Comand not defined\n\n"  
        wprint "$format" " " "Command" "Description\n"          
    else
        format="%-${szN}s${BWHITE}%-${szC}s${STDCL} - %-s"
        wprint "$format" " " "${cmd}" "${HelpCnt[$cmd]}\n"  
    fi
}

HelpAll()
{
    for key in $lkeys; do Helpline "$sh1" "$sh2" "$key"; done
}

isakey()
{
    local key
    for key in ${!HelpCnt[@]}; do 
        if [[ "$key" == "$1" ]]; then
            echo -n "YES"
            return
        fi
    done
    echo -n "NO"
}

Help()
{
    local subcmd="$1" sh1 sh2 
    let sh1=${#NAMEPRG} 
    let sh2=0

    wprint "\n ${BWHITE}$NAMEPRG${STDCL} ver. $VERPRG - ${Annotation}\n Usage: " 
    for fmtcmd in "${CommonFormat[@]}"; do wprint " ${BWHITE}$NAMEPRG${STDCL} $fmtcmd\n"; done
    HelpSubCmd "help" "$NAMEPRG help [subcomand]"
    lkeys="$(for s in ${!HelpCnt[@]}; do echo "$s"; done | sort)"
    for cmd in $lkeys; do (( "$sh2" <= "$(strlen $cmd)" )) && sh2=$(strlen "$cmd"); done

    if [[ -n "$subcmd" ]]; then
        Helpline "$sh1" "2" "$subcmd" 
        [[ $(isakey $subcmd) == "NO" ]] && HelpAll
    else
        HelpAll
    fi
    echo
}
