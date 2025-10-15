############################################################################
##
## help part - services function
##
## wocson (c) 2012 BSD
##
############################################################################
NAMEPKG="helpsys"
VERPKG="0.10.43"

declare -a HelpCnt
declare -A HelpCmd
declare -a HelpNts

NAMEPGR="(unset)"
VERPRG="(unset)"
Annotation="(unset)"
let iCnt=0
let iNts=0
cmdm=0

getVerprg()
{
    echo -en "$VERPRG"
}

helpline()
{
    let szN="$1+2" szC="$2"
    local cmd="$3" mcmd="$4 "

    if [[ $(isakey $cmd) != "NO" ]]; then
        format="%-${szN}s${BWHITE}%-${szC}s${STDCL} - %-s"
        wprint "$format" "" "${mcmd}${cmd}" "${HelpCmd[$cmd]}\n"  
    fi
}

helpAll()
{
    for fmtcmd in "${HelpCnt[@]}"; do wprint " $fmtcmd\n"; done
    lkeys="$(for s in ${!HelpCmd[@]}; do echo "$s"; done | sort)"
    for cmd in $lkeys; do (( "$sh2" <= "$(strlen $cmd)" )) && sh2=$(strlen "$cmd"); done
    for key in $lkeys; do helpline "$sh1" "$sh2" "$key"; done
}

isakey()
{
    local key
    for key in ${!HelpCmd[@]}; do 
        if [[ "$key" == "$1" ]]; then
            echo -n "YES"
            return
        fi
    done
    echo -n "NO"
}

HelpAddAntn() #1
{
    Annotation="$1" 
}

HelpAddMCmd() #2+Mix
{
    local description="$1"

    HelpCnt[$iCnt]="${BWHITE}$NAMEPRG${STDCL} $description" 
    let iCnt++
}

HelpAddCmnt() #3+Mix
{
    local description="$1" tab="3" smbl=" "
    tab=$(printf "${smbl}%.0s" $(seq $tab)) 
    HelpCnt[$iCnt]="${tab}$description" 
    let iCnt++
}

HelpAddSCmd() #4+
{
    local key="$1" description="$2"
    key=$(echo $key | cut -f1 -d'|')

    if [[ "$cmdm" == "0" ]]; then HelpCnt[$iCnt]="${WHITE}command: "; let iCnt++; cmdm=1; fi

    [[ "$(isakey $key)" == "NO" ]] && HelpCmd["$key"]="$description" 
}

HelpAddNts() #End
{
    local description="$1"

    HelpNts[$iNts]="\n${GREEN}Notes:${STDCL}  $description" 
    let iNts++
}

Help()
{
    local subcmd="$1" sh1=${#NAMEPRG} sh2=0

    HelpAddSCmd "help" "$NAMEPRG help [command]"
    wprint " ${BWHITE}${NAMEPRG}${STDCL} ver. $VERPRG - ${Annotation}\n Usage: \n" 

    if [[ -n "$subcmd" ]]; then
        helpline "$sh1" "4" "$subcmd" "${NAMEPRG}"
        [[ $(isakey $subcmd) == "NO" ]] && helpAll
    else

        helpAll
        for note in "${HelpNts[@]}"; do wprint " $note\n"; done
    fi
    wprint "%$((TC))s\n" " wocson (c) 2008"
}