############################################################################
##
## libpdmn - function for work whit Packages / mv cp del
##
## wocson (c) 2025 BSD
##
############################################################################
. "$HOME"/.local/share/shells/lib/services.sh

NAMEPRG="libpdmn"; VERPRG="0.10.31"; [[ $DEBUG == ON ]] && CMNT="first add" 
[[ $DEBUG == ON ]] && wprint "${BGRAY}$NAMEPRG v.$VERPRG\n"

#== GLOBAL VARS ===========================================================

MAINTAINERDIR="$HOME/Maintainer"
REPOADIR="$MAINTAINERDIR"/repo-automation
SBERFDIR="$MAINTAINERDIR"/sberos-factory
RETSEL="Cancel"

#== FN ====================================================================

Selector()
{
    local LIST="$1" EXITP="$2"
    [[ -z "$2" ]] && EXITP="Cancel"
    LIST="$LIST $EXITP"

    select RETSEL in $LIST; do
        if [[ "$RETSEL" != "$EXITP" ]]; then
            break
        else
            wprint "Exit\n"
            QUIT 11
        fi
    done
}

QUIT()
{
    local MSG="$2"
    [[ -z $MSG ]] && MSG="${BGREEN}Done"
    wprint "$MSG\n"
    exit "$1"
}

CorTaskFileName()
{
    local TF0="$1" TF="$1" SFX="$2" nf=0

    if [[ -n $2 ]]; then 
        while true ; do
            if [[ -f "$TF" ]]; then 
                let nf+=1
                TF="${TF0}_${SFX}${nf}"
            else 
                break
            fi
        done
    fi
    echo -n "$TF"    
}

crlog()
{
    echo -e "$MSRC:" > "$LOGT"
    cat "$TASKPKGDIR"/${NAME}.cp >> "$LOGT"
    echo -e "\n$MDST:" >> "$LOGT"
    cat "$TASKPKGDIR"/${NAME}.rm >> "$LOGT"
}

UpdateMetaData()
{
    [[ -z $REPOSMETA ]] && QUIT 11 "${ERR}Don't set list of repository for get metadata!"

    yesno "Update" "Update repo and metadata"
    case ${UCH} in
    0)
        wprint "${BGREEN}Set branch: $BRANCH\n"; 
        git switch "$BRANCH"; 
        git pull
        get_metadata.sh $REPOSMETA
    ;;
    *)
        return
    ;;
    esac
}

## 
##  BRANCH - branch 
##  SRCPRFILE, 
##  DSTPRFILE - file name with fullpath 
##  TASK - Are give name task from Kanban board
##
Commit()
{
    local BRANCH="$1" SRCPRFILE="$2" DSTPRFILE=$3 TASK="$4" PATHCMD="$5"
    local DSTLCFNAME=""
    local SRCFN=$(echo "$SRCPRFILE" | rev | cut -f1 -d'/' | rev)
    local DSTFN=$(echo "$DSTPRFILE" | rev | cut -f1 -d'/' | rev)

    [[ -z $TASK ]] && TASK="SBOS-2037"
    [[ -z $PATHCMD ]] && PATHCMD="."

    rm -fv "${PATHCMD}/sendpr"

    wprint "\n${BGREEN} Send Task: ${BWHITE}${TASK}${BGREEN} to PR:\n $SRCPRFILE => $DSTPRFILE\n\n"

    advice="
cd ${SBERFDIR}
branch=${BRANCH}; git checkout SBOS-2037/\$branch && git pull upstream SBOS-2037/\$branch
cp -fv ${SRCPRFILE} ${DSTPRFILE}
git pull
git add var/db/nva/${DSTFN}
git commit -m ${DSTFN}
git push"

    echo -e "#!/bin/bash\n" > "${PATHCMD}/sendpr"
    echo "$advice" >> "${PATHCMD}/sendpr"
    chmod u+x "${PATHCMD}/sendpr"

    yesno "Send PR" "Send PR to Factory?" 'n'
    case ${UCH} in
    0)
        wprint "\n${BGREEN}Send PR to Factory run \'"${PATHCMD}"/sendpr\'\n"
    ;;
    *)
        wprint "\n${BGREEN}Manualy run this commands:\n"
        wprint "Or run \'"${PATHCMD}"/sendpr\'\n\n"
    ;;
    esac
    wprint "${BWHITE}$advice\n\n"
}

## Create new repo
##
## use API need get TOKEN
##
## broken
##
CreateNewRepo() 
{
    local URL="https://stash.delta.sbrf.ru/${API}/users/$USER/repos"
    local REPONAME="$1"
    
    curl -X POST -v \
 -H "Authorization: Bearer $TOKEN" \
 -H "Content-Type:application/json" \
 -d "{\"name\": \"$REPONAME\", \"scmId\": \"git\", \"forkable\": \"false\" }" "$URL" > "$TMPHTML"
    
    cat "$TMPHTML" | jq

#git remote add fork git@stash.delta.sbrf.ru:7999/~22969712/test001.git
#git push -u fork --all
#git push -u fork --tags
}

#
#  catmsgs "Title|linecolor|linetype" "title1|note1/color|message1" ... "titleN|noteN/color|messageN"
#
catmsgs()
{
    local TLE="$(echo "$1|" | cut -sf1 -d'|')" LCL="$(echo "$1" | cut -sf2 -d'|')" LNT="$(echo "$1" | cut -sf3 -d'|')"
    local TL="" CL="" NF=""

    [[ -z "$TLE" ]] && TLE=""
    [[ -z "$LCL" ]] && LCL=BGREEN
    [[ -z "$LNT" ]] && LNT=""     

    Hline "$TLE" "${!LCL}" "$LNT"

    shift 1
    for arg in "$@"; do 
        TL=$(echo $arg | cut -f1  -d'|'); [[ -n $TL ]] && wprint "${TL}\n"
        CL=$(echo $arg | cut -f2  -d'|'); [[ -z $CL ]] && CL="WHITE"
        NF=$(echo $arg | cut -f3- -d'|' | tr " " "\n"); [[ -n $NF ]] && wprint "${!CL}${NF}\n"
    done
    Hline "" "${!LCL}" 
}
