################################################################
##
## copy shells for all users on system
## wocson (c) lic. BSD
##
################################################################
OPTYPE="$1"
sudoup="sudo"

. ./inst.conf
. ./bin/zfncode.sh

case "$OPTYPE" in 
user)
    echo "Install for user"

    defansw="$USER"
    HOMEU=$(echo $HOME | rev | cut -f1 -d'/' | rev)

    if [[ "$USER" != "$HOMEU" ]]; then
        defansw="$(echo $USER | cut -f2 -d'@')@$(echo $USER | cut -f1 -d'@')"
    fi

    read -p "Home dir for install [$defansw]: " answ; answ=${answ:-$defansw}
    homedir=$answ
    cpsh2user "/home/$homedir" "$USER"    
    
;;
users)
    echo "Install for all users exept root"
    warning $OPTYPE

    defansw="n"
    read -p "Your install env for all users in this system. Your sure?(Yes/n) [$defansw]: " answ; answ=${answ:-$defansw}

    if [[ "$answ" == "Yes" ]]; then
        cpsh2users 
    else 
        echo "Your answear '$answ'. Break. Sorry"
        exit 11
    fi
    finish
;;
root)
    echo "Install for root"
    warning $OPTYPE

    defansw="n"
    read -p "Your install env and shell for root. Your sure?(Yes/n) [$defansw]: " answ; answ=${answ:-$defansw}

    if [[ "$answ" == "Yes" ]]; then
        cpsh2user "/root" "root"
    else 
        echo "Your answear '$answ'. Break. Sorry"
        exit 11
    fi
    finish
;;
system)
    echo "Install for ALL users on system"
    warning $OPTYPE

    defansw="n"
    read -p "Your install env for in this system. Your sure?(Yes/n) [$defansw]: " answ; answ=${answ:-$defansw}

    if [[ "$answ" == "Yes" ]]; then
        cpsh2user "/root" "root"
        cpsh2users 
        copy2skel
    else 
        echo "Your answear '$answ'. Break. Sorry"
        exit 11
    fi

    finish
    echo "Done for ALL."
;;
backup)
    echo "Backup to  '$PATH2SAVE"    
    zbackup
;;
*)
    echo "Unknown '$1' operation. Sorry. Exit"    
    exit 11
;;
esac

if [[ "$(uname)" == "FreeBSD" && $sudoup == sudo ]]; then
    $sudoup cap_mkdb /etc/login.conf
fi

exit 0
