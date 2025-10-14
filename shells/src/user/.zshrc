##############################################################################
##
## .zshrc - resource script, read at beginning of execution by each shell
##
## wocson (c) 2008 BSD
##
## for user
##
##############################################################################
##
## Solution:
## fstat /dev/dsp* /dev/mixer* | tail -n +2 | awk '{print $3}' | xargs sh -c 'sudo kill -9 $0 $*'
##
LIBINCPATH="$HOME/.local/share/shells"
#PROXY="10.31.80.15:3128"

source $LIBINCPATH/zshrc.sh
source $LIBINCPATH/bzfns.sh

##
## Set proxy
##
#proxy set $PROXY

## set locale
utf > /dev/null

## ENVIRONMENT VARIABLES
export PATH="$PATH:$HOME/bin"
#export PKG_FETCH="/usr/local/bin/wget -O '%2\$s' '%1\$s'"
#export FORCE_PKG_REGISTER=YES
#export PACKAGEROOT=ftp://ftp.corbina.ru
#export NEWSSERVER=your.news.server

[ -f $(which most) ] && export PAGER=most    || export PAGER=less
[ -f $(which mc)   ] && export EDITOR=mcedit || export EDITOR=vim

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib
 
HISTSIZE=5000
SAVEHIST=5000
umask 077
