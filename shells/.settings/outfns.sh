############################################################################
##
##  function include
##
##  wocson (c) 2024 lic. BSD
##
############################################################################
#TRACE(){ echo -e "TRACE: $1 $2 $3 $4 $5 $6 $7 $8 $9\n"; }

retcd=""

Warning()
{
cat << EOF
    For install shells environments bash tcsh zsh needs:
    1. THE ROOT ACCESS LEVEL IS REQUIRED - 'sudo' and you member group 
       'wheel/root' or 'su' and root password for access."
    2. Access to the internet or local software repositories to install
       these programs:
       $LIST4INST
    3. A little luck.

    If you not sudo and not have records on sudo will be 
    request password.
EOF
}

checkonsu()
{
    local fsudoers="/usr/local/etc/sudoers"

    if ! [ -f "$fsudoers" ]; then
        fsudoers="/etc/sudoers"
    fi

    local optpackman="install -y $LIST4INST"
    local editsudoers="sed -i '' 's/# %wheel ALL=(ALL:ALL) NOPASSWD: ALL/\%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' $fsudoers"
    
    if [ $CHECKPKGS == "YES" ]; then
        checkerpkg="$Packman $optpackman"
        su -l root -c "$checkerpkg ; $editsudoers"
    else 
        su -l root -c "$editsudoers"
    fi

    retcd="$?"
}

Checkallpermissions()
{
    local user=$1 fdir=$2

    echo "Check permission... to $user"
    group="$(id -gn "$user")"
    echo "User have permission: ${user}:${group}"

    if [ "$HOME" == "$fdir" ]; then return; fi

    if [ -f "$(which sudo)" ]; then
        sudo -n ls $fdir > /dev/null   
        retcd="$?"
        if [ $retcd -ne 0 ]; then
            echo "Check Your permissions."
            exit 11
        else
            return
        fi
    else
        return
    fi
}

strcnt(){ [ -z "$1" ] || { [ -z "${2##*$1*}" ] && [ -n "$2" ];}; }
