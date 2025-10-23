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
    1. THE ROOT ACCESS LEVEL IS REQUIRED. Or 'sudo' and you member group 
       'wheel/root' or 'su' and root password for access."
    2. Access to the internet or local software repositories to install
       these software:
       $LIST4INST

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

strcnt(){ [ -z "$1" ] || { [ -z "${2##*$1*}" ] && [ -n "$2" ];}; }

cleanprev()
{
    local FPATHDEL="$1"

    if [[ -d "$FPATHDEL"/shells ]]; then
        $sudoup rm -R "$FPATHDEL"/.config/shells &> /dev/null
    fi
}

warning()
{
if [ "$OPTYPE" == "root" ] || [ "$OPTYPE" == "ALL" ] ; then
cat << EOF

    Changing the environment for root is usually not performed. 
    This user is left with the default settings. The change may 
    create unforeseen problems.
EOF
fi

[ "$OPTYPE" == "ALL" ] && echo "    and";

if [ "$OPTYPE" == "users" ] || [ "$OPTYPE" == "ALL" ]; then 
cat << EOF

    Environment changes for all users in the system (except root).
EOF
fi
cat << EOF

    You should make changes only if you are completely 
    confident in your actions.
EOF
}

place2()
{
    local username="$1"

    if [[ $username == "$USER" ]]; then 
        sudoup=""; 
    else
        sudoup="sudo"; 
    fi    

    $sudoup rsync -a "$2" "$3"
}

##
## Save Before
##
savebefore()
{
    echo -e  "Save before... "      

    local HOMEPATH="$1"
    local SHELLSDIR="$HOMEPATH"/".config/shells"
    local SAVEBEFOR="$SHELLSDIR"/"save_before"
    local DATE="$(date +'sh-%Y%m%d-%H%M%S')"
    local PACKDIR="$SAVEBEFOR"/"$DATE"
    local NameBackup="$SAVEBEFOR"/"$DATE".tar.bz2

    echo HOMEPATH   "$HOMEPATH"
    echo SHELLSDIR  "$SHELLSDIR"
    echo SAVEBEFOR  "$SAVEBEFOR"
    echo DATE       "$DATE"
    echo PACKDIR    "$PACKDIR"
    echo NameBackup "$NameBackup"
 
    if mkdir -p $PACKDIR; then

        cp "$HOMEPATH"/.bash* "$PACKDIR"/
        cp "$HOMEPATH"/.*csh* "$PACKDIR"/
        cp "$HOMEPATH"/.zsh* "$PACKDIR"/
        cp "$HOMEPATH"/.shrc "$PACKDIR"/

        echo -e "Store to:\n\t$NameBackup"
        tar cf - "$PACKDIR" | bzip2 -z > "$NameBackup"
        rm -R "$PACKDIR"
    else
        echo "Error create dir for save a before shells config"
    fi
}

##
## cpsh2user "homedir" "user"
##
cpsh2user()
{
    local fdir="$1" username="$2"

    echo "Copy to $fdir"
    
    if [ $(id $username | echo $?) -eq 0 ]; then

        echo "Path to install: $fdir"
        savebefore "$fdir"
        echo -en "Copy... "        
 
        cleanprev "$fdir" 
        place2 "$username" "./src/common/" "$fdir"/
        
        if [[ "$username" == "root" ]]; then
            place2 "$username" "./src/root/" "$fdir"/
        else 
            place2 "$username" "./src/user/" "$fdir"/
            if [[ "$XFILES" == "YES" ]]; then
                place2 "$username" "./src/xfiles/" "$fdir"/
            fi
        fi
        
        echo -en "Set permission... "
        sudo chown -Rf $uid:$gid "$fdir" 
        sudo chmod -f 700 "$fdir" 
        echo -en "Done\n\n"
    else
       echo "ERROR: User: $username not find on system."
    fi
}

cpsh2users()
{
    echo "Copy shells for users on system."
    echo "Directory: $DIRECTORY"
    if [[ -n "$WINDOMAIN" ]]; then echo "Windomain: $WINDOMAIN"; else echo -en "Windomain: none\n\n"; fi

    if [[ -d "$DIRECTORY" ]]; then
        for fdir in $DIRECTORY/* ; do
            if [ -d $fdir ] ; then
    	        username="$WINDOMAIN"`echo "$fdir" | rev | cut -d'/' -f1 | cut -d'(' -f1 | rev`
                cpsh2user "$fdir" "$username"
            fi    
        done
    else
        echo "Not find $DIRECTORY"
    fi
}

copy2skel()
{
    if strcnt "BSD" $(uname) ; then perm=root:wheel;
    elif strcnt "Linux" $(uname) ; then perm=root:root;
    else echo "Unknown OS";  exit 1;
    fi

    username=root

    # Clean skel
    echo "Clean system skel"
    sudo rm -R "/usr/share/skel/"* &> /dev/null
    sudo rm -R "/usr/share/skel/".* &> /dev/null
    sudo rm -R "/etc/skel/"* &> /dev/null
    sudo rm -R "/etc/skel/".* &> /dev/null

    echo "Copy to skel:"
    echo "    /etc/skel /etc\n"

    # Copy to skel
    sudo chown -Rf $perm ./etc/
    place2 "$username" "./src/etc/" "/etc/"
    place2 "$username" "./src/common/" "/etc/skel/"
    place2 "$username" "./src/user/" "/etc/skel/"
    if [[ "$XFILES" == "YES" ]]; then
        place2 "$username" "./src/xfiles/" "/etc/skel/"
    fi

    echo "    /usr/share/skel\n"

    place2 "$username" "./src/common/" "/usr/share/skel/"
    place2 "$username" "./src/user/" "/usr/share/skel/"
    if [[ "$XFILES" == "YES" ]]; then
        place2 "$username" "./src/xfiles/" "/usr/share/skel/"
    fi

    sudo mv -f /usr/share/skel/.Xresources /usr/share/skel/dot.Xresources
    sudo mv -f /usr/share/skel/.bash_logout /usr/share/skel/dot.bash_logout
    sudo mv -f /usr/share/skel/.bash_profile /usr/share/skel/dot.bash_profile
    sudo mv -f /usr/share/skel/.bashrc /usr/share/skel/dot.bashrc
                                   
    sudo mv -f /usr/share/skel/.tcshrc /usr/share/skel/dot.tcshrc
    sudo mv -f /usr/share/skel/.zshrc /usr/share/skel/dot.zshrc
    sudo mv -f /usr/share/skel/.shrc /usr/share/skel/dot.shrc
    sudo mv -f /usr/share/skel/.profile /usr/share/skel/dot.profile

    # set perm on local setup dirs
    echo "Set permission... "

    sudo chown -Rf $perm /etc
    sudo chown -f $perm /etc/csh*
    sudo chown -f $perm /etc/zl*
    sudo chown -Rf $perm /usr/share/skel

#    sudo chown -Rf $perm /etc

    echo -en "Copy to skel done\n\n"
}
    
finish()
{
    if [[ -f /usr/bin/bzip2 ]]; then 
        sudo mv -f /usr/bin/bzip2 /usr/bin/bzip2_old
    fi

    if strcnt "BSD" $(uname) ; then
        sudo ln -sf /usr/local/bin/zsh /bin/zsh
        sudo ln -sf /usr/local/bin/bash /bin/bash
        sudo ln -sf /usr/local/bin/pbzip2 /usr/bin/bzip2
        sudo ln -sf /etc/zlogin /usr/local/etc/zlogin
        sudo ln -sf /etc/zlogout /usr/local/etc/zlogout

        if ! [[ -f /usr/local/bin/7za ]]; then 
            sudo ln -sf /usr/local/bin/7zz /usr/local/bin/7za
        fi

    elif strcnt "Linux" $(uname) ; then 
        sudo ln -sf /usr/bin/pbzip2 /usr/bin/bzip2

        if ! [[ -f /usr/bin/7zz ]]; then 
            sudo ln -sf /usr/bin/7zz /usr/bin/7za
        fi
    fi
}

checkinstposs()
{
    local count=1 

    echo "Installed needed packets"
    echo "Please input root password..."

    while true; do

    if [ -f "$(which sudo)" ]; then
        sudo $Packman install -y $LIST4INST
        retcd="$?"
        if [ $retcd -ne 0 ]; then
            checkonsu
        fi
    else
        defansw="Y"
        read -p "You have needed permissions? (Y/n) [$defansw]: " answ; answ=${answ:-$defansw}

        if [ $answ == "Y" ] || [ $answ == "y" ]; then
            checkonsu
        else
            echo "Can't installed shells. Exit."
            exit 8 # not pass permission
        fi
        [ $count -gt 2 ] && break;
    fi

    [ $retcd -eq 0 ] && break;
    count=$(let count+1)
    done
}

zbackup()
{
    if ! [[ -d "$PATH2SAVE" ]]; then

        echo -e "\tNot find path: \n\t$PATH2SAVE"
        PATH2SAVE=""
        defansw="$HOME/tmp/Backups"
        
        read -p "Name for install [$defansw]: " answ; answ=${answ:-$defansw}
        PATH2SAVE=$answ

        if mkdir -p "$PATH2SAVE"; then
            echo -e "Create path:\n\t$PATH2SAVE"
        else
            echo -e "Do not create path:\n\t$PATH2SAVE"
            return
        fi

    fi

    Pwd=$(basename $PWD)
    tar="$Pwd"
    Date=$(date +'%Y%m%d_%H%M%S')
    Ver=$(grep "version:" ./readme | cut -f2 -d':' | cut -c2-)
    NameBackup="$tar-$Ver-$Date.tar.bz2"
    echo -e "Store to:\n\t$NameBackup"
    tar cf - "$PWD" | bzip2 -z > ""$PATH2SAVE"/$NameBackup"
}
