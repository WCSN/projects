############################################################################
##
## libbash part - services function
##
## wocson (c) 2012 BSD
##
############################################################################
NAMEPKG="libbash"
VERPKG="0.10.31"

. $HOME/.local/share/shells/lib/tcolors.sh
. $HOME/.local/share/shells/lib/helpsys.sh

ARGV[0]="$0"; let i=1; for arg in "$@"; do ARGV[$i]="$arg"; let i++; done
ARGC=${#ARGV[@]}
ANSW=""

TL=`tput lines`
TC=`tput cols`

gX=0
gY=0

DialogType="text" # window/text
UCH="255"
DIALOG=${DIALOG=dialog}


gotoxy()
{
    echo -en "\033[$2;$1"; echo -en 'f'
}


getcolor()
{
  local CL="$1";  echo -en "${!CL}"
}


setcolor()
{
    echo -en "$1"
}

setbackcolor()
{
    echo -en $1
}

clrscr()
{
    echo -en $CLSTD
    clear
}

outtextxy()
{
    gotoxy $1 $2
    echo -en "$3"
}

_outtextxy()
{
  gotoxy $1 $2 ;
  if [ -z "$4" ]; then format="%s";
  else format="$4"; fi
  echo "printf \"$format\" \"$3\"" | zsh
}

strlen()
{
    local string="$1"
    echo -en "${#string}"
}

getxy()
{
  local pos
  IFS='[;' read -p $'\e[6n' -d R -a pos -rs || echo "failed with error: $? ; ${pos[*]}"
  gX="${pos[2]}"
  gY="${pos[1]}"
}

getcursorxy()
{
  local pos
  IFS='[;' read -p $'\e[6n' -d R -a pos -rs || echo "failed with error: $? ; ${pos[*]}"
  gX=${pos[2]}
  gY=${pos[1]}
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
    [[ $DEBUG != ON ]] && return;
    
    echo -en "${BRED}";   printf "%-10s" "TRACE ${1}"
    echo -en "${YELLOW}"; printf "=%.0s" $(seq $((TC-11)));
    echo -en "\n" 

    shift 1
    for arg in $@; do 
        echo -en "${YELLOW}==> ${BGREEN}${arg}: ${STDCL}${!arg}${STDCL}\n"
    done 
    echo -en "${YELLOW}"; printf "=%.0s" $(seq $((TC-1))); echo -en "\n"
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

choice()
{
    local answ="" TEXT="$1" Vansw="$2" Defansw="$3" 
    local LSENTENS="$TEXT"; 
    
    [[ -n $Vansw   ]] && LSENTENS="${LSENTENS} ($Vansw)"
    [[ -n $Defansw ]] && LSENTENS="${LSENTENS}[$Defansw]"

    read -p " ${LSENTENS}: " answ; answ=${answ:-$Defansw}
    echo -n "$answ"
}

yesno()
{
    local Title="$1" Question="$2" defansw="$3"
    [[ -z $defansw ]] && defansw='n'
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
        case $(choice ">> $Question" "y/n" "$defansw") in
        y|Y) UCH="0";;
        n|N) UCH="1";;
          *) UCH="255";;
        esac
    fi
}

##
## $1 - Colors
## $2 - Title
## $3 - Symbol for line
##
Hline()
{ 
    local TITLE="" HCLR="${BGREEN}" symbln="─" 

    [[ -n "$1" ]] &&  TITLE="$1"
    [[ -n "$2" ]] &&   HCLR="$2"  
    [[ -n "$3" ]] && symbln="$3"                      

    echo -en "${HCLR}"; wprint "${symbln}%.0s" $(seq $TC); 
    getxy; gotoxy 3 $gY; 
    echo -en "${TITLE}${BREAK}\n"
}

#
# boxInfo "string1" "string2" "looooooooooooooooooooooooooongstring"
#
ODTITLEBOX=""
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

