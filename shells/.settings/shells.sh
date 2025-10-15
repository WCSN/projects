################################################################
##
## copy shells for all users on system
## wocson (c) lic. BSD
##
################################################################
. $HOME/.local/share/shells/lib/services.sh
. ./.settings/shells.conf
. ./.settings/outfns.sh

OPTYPE="$1"
packman="$2"
sudoup="sudo"

cleanprev()
{
    local FPATHDEL="$1/.local/share"

    for fname in $LIST4DEL; do 
        [[ -f "$FPATHDEL/$fname" ]] && $sudoup rm "$FPATHDEL/$fname" &> /dev/null 
    done

    [[ -d "$FPATHDEL"/shells ]] && $sudoup rm -R "$FPATHDEL"/shells &> /dev/null
}

warning()
{
    if [ "$OPTYPE" == "root" ] || [ "$OPTYPE" == "system" ] ; then
cat << EOF

    Changing the environment for root is usually not performed. 
    This user is left with the default settings. The change may 
    create unforeseen problems.

EOF
    fi

    [[ "$OPTYPE" == "system" ]] && echo "    and";

    if [ "$OPTYPE" == "users" ] || [ "$OPTYPE" == "system" ]; then 
cat << EOF

    Environment changes for all users in the system (except root).

EOF
    fi
cat << EOF

    You should make changes only if you are completely 
    confident in your actions.

EOF
}

##
## cpsh2user "homedir" "user"
##
cpsh2user()
{
    local fdir="$1" username="$2"
    
    if [ $(id $username | echo $?) -eq 0 ]; then

        uid=$(id $username | cut -d'=' -f2 | cut -d'(' -f1)
        gid=$(id $username | cut -d'=' -f3 | cut -d'(' -f1)
        
        echo -e "Copy for User: ${BGREEN}${username} ${STDCL}\nID: ${BBLUE}$(id $username)${STDCL}\nPath: ${BWHITE}$fdir${STDCL}"
        
        [[ $username == "$USER" ]] && sudoup="" || sudoup="sudo"; 

        cleanprev "$fdir" 
        $sudoup rsync -a ./src/common/ "$fdir"/ 
       
        if [[ "$username" == "root" ]]; then
            $sudoup rsync -a ./src/root/ "$fdir"
        else 
            $sudoup rsync -a ./src/user/ "$fdir"
            [[ "$XFILES" == "YES" ]] && $sudoup rsync -a ./src/xfiles/ "$fdir"
        fi
        
        $sudoup chown -Rf $uid:$gid "$fdir" 
        $sudoup chmod -f 700 "$fdir" 
        echo -e "Ready\n"
    else
       echo -e "${ERR} ERROR: User $username not find on system. "
    fi
}

cpsh2users()
{
    echo "Copy shells for users on system."
    echo "Directory: $DIRECTORY"
    
    [[ -n "$WINDOMAIN" ]] && echo "Windomain: $WINDOMAIN" ||  echo -en "Windomain: none\n\n"

    if [[ -d "$DIRECTORY" ]]; then
        for fdir in $DIRECTORY/* ; do
            if [ -d $fdir ] ; then
    	        username=${WINDOMAIN}$(echo "$fdir" | rev | cut -d'/' -f1 | cut -d'(' -f1 | rev)
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

    # Clean skel
    echo "Clean system skel"
    sudo rm -R "/usr/share/skel/"* &> /dev/null
    sudo rm -R "/usr/share/skel/".* &> /dev/null
    sudo rm -R "/etc/skel/"* &> /dev/null
    sudo rm -R "/etc/skel/".* &> /dev/null

    echo -e "Copy to:\n\t/etc/skel"

    # Copy to skel
    sudo chown -Rf $perm ./src/etc/
    sudo rsync -a ./src/etc/ /etc/
    sudo rsync -a ./src/common/ /etc/skel/
    sudo rsync -a ./src/user/ /etc/skel/
    [[ "$XFILES" == "YES" ]] && sudo rsync -a ./src/xfiles/ /etc/skel/
    
    echo -e "\t/usr/share/skel"

    sudo rsync -a ./src/common/ /usr/share/skel/
    sudo rsync -a ./src/user/ /usr/share/skel/
    
    sudo mv -f /usr/share/skel/.bash_logout /usr/share/skel/dot.bash_logout
    sudo mv -f /usr/share/skel/.bash_profile /usr/share/skel/dot.bash_profile
    sudo mv -f /usr/share/skel/.bashrc /usr/share/skel/dot.bashrc
    sudo mv -f /usr/share/skel/.tcshrc /usr/share/skel/dot.tcshrc
    sudo mv -f /usr/share/skel/.zshrc /usr/share/skel/dot.zshrc
    sudo mv -f /usr/share/skel/.shrc /usr/share/skel/dot.shrc
    sudo mv -f /usr/share/skel/.profile /usr/share/skel/dot.profile

    # set perm on local setup dirs
    echo "Set permission: $perm"

    sudo chown -Rf $perm /etc
    sudo chown -f $perm /etc/csh*
    sudo chown -f $perm /etc/zl*
    sudo chown -Rf $perm /usr/share/skel
}
    
finish()
{
    [[ -f /usr/bin/bzip2 ]] && sudo mv -f /usr/bin/bzip2 /usr/bin/bzip2_old

    if strcnt "BSD" $(uname) ; then
        sudo ln -sf /usr/local/bin/zsh /bin/zsh
        sudo ln -sf /usr/local/bin/bash /bin/bash
        sudo ln -sf /usr/local/bin/pbzip2 /usr/bin/bzip2
        sudo ln -sf /etc/zlogin /usr/local/etc/zlogin
        sudo ln -sf /etc/zlogout /usr/local/etc/zlogout

        [[ -f /usr/local/bin/7za ]] || sudo ln -sf /usr/local/bin/7zz /usr/local/bin/7za

    elif strcnt "Linux" $(uname) ; then 
        sudo ln -sf /usr/bin/pbzip2 /usr/bin/bzip2

        [[ -f /usr/bin/7zz ]] || sudo ln -sf /usr/bin/7zz /usr/bin/7za
    fi
}

case "$OPTYPE" in 
user)
    echo "Install for user"

    defansw="$USER"
    read -p "Name for install [$defansw]: " answ; answ=${answ:-$defansw}
    user=$answ
    
    if [[ "$USER" != "$user" ]]; then
        defansw="n"
        read -p "You are changing the environment for another user: $answ. Your sure?(Yes/n) [$defansw]: " answ; answ=${answ:-$defansw}
        if [[ "$answ" == "Yes" ]]; then
            cpsh2user "/home/$user" "$user"
        else
            echo "Your answear '$answ'. Break. Sorry"
            exit 11
        fi
    else
       cpsh2user "/home/$user" "$user"    
    fi
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
    echo "Install for System"
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
;;
build)
    cp -afT ./.settings/.mc.menu.gen ./.mc.menu
;;
pull)
	git pull
;;
commit)
	git add --all 
    git commit -m "correct shells $(date +%Y%m%D-%H%M%S)"
    git push
    ./.settings/save
;;
*)
    echo "Unknown '$1' operation. Sorry. Exit"    
    exit 11
;;
esac

[[ "$(uname)" == "FreeBSD" && $sudoup == sudo ]] && $sudoup cap_mkdb /etc/login.conf

echo "Install shells env Done"

exit 0
