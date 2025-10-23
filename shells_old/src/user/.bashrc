##############################################################################
##
## .zshrc - - resource script, read at beginning of execution by each shell
## wocson (c) 2024
##
## for user
##
##############################################################################
##
## Solution:
## fstat /dev/dsp* /dev/mixer* | tail -n +2 | awk '{print $3}' | xargs sh -c 'sudo kill -9 $0 $*'
##
LIBINCPATH="$HOME/.config/shells"
#PROXY="10.31.80.15:3128"

source $LIBINCPATH/zshrc.d/bashrc.sh
source $LIBINCPATH/share/shellfns.sh

##
## Set proxy
##
#proxy set $PROXY

## set locale
utf > /dev/null

## ENVIRONMENT VARIABLES
export PATH="$HOME/bin:$PATH"
#export PKG_FETCH="/usr/local/bin/wget -O '%2\$s' '%1\$s'"
#export FORCE_PKG_REGISTER=YES
#export PACKAGEROOT=ftp://ftp.corbina.ru
#export PACKAGESITE=$PACKAGEROOT/pub/FreeBSD/ports/`uname -p`/packages-9-stable/All/
#export NEWSSERVER=your.news.server

if [ -f $(which most) ]; then
    export PAGER=most
else
    export PAGER=less
fi

if [ -f $(which mc) ]; then
    export EDITOR=mcedit
else
    export EDITOR=vim
fi    

HISTSIZE=5000
SAVEHIST=5000
#f_copy
#f_variables
umask 077
