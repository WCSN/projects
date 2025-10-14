#!/bin/bash
#################################################################
##
## AARI
##
## setup
##
## wocson (c)
##
#################################################################
CMD="$1"


function CleanPrj()
{
    make clean

    if [[ -d "brga" ]]; then
        sudo rm -R "./brga/release"
        sudo rm -R "./brga/debug"
        sudo rm -R "./brga/moc"
        sudo rm ./brga/Makefile*
        sudo rm "./brga/brga.pro"
        sudo rm ./Makefile*
        sudo rm ./aari*
        sudo rm ./.gitignore
        sudo rm -R ./.qmake*        
        sudo rm -R ./.qtc_*
    fi
} 

function Commit()
{
    defmsg="correct"
    read -p "Message for commit ($defmsg): " msg; msg=${msg:-$defmsg};

    if [[ -n $(whereis svn | grep -i "bin" | cut -f2 -d':') ]]; then    
        echo "== SVN ====================="    
        if [[ -d "./.svn" ]]; then
            svn commit --force-log -m "$msg"
        else
            echo "SVN not prepare..."
        fi
    fi

    if [[ -n $(whereis git | grep -i "bin" | cut -f2 -d':') ]]; then
        echo "== GIT ====================="        
        if [[ -d "./.git" ]]; then
            git commit -a -m "$msg"
            git push -v -u origin main
        else
            echo "GIT not prepare..."
        fi
    fi
}

#=================================================================

QUIT="NO"

case "$CMD" in
commit)
    Commit
;;
cleanall)
    CleanPrj
;;
svnupdate)
    svn update
;;
gitupdate)
    git pull
;;
*)
    echo -en "\n usage: setup [ commit | clean ]\n\n"
;;
esac

exit 0
