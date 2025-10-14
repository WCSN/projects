####################################
##
## .zshrc
## (c) wocson
##

##
## Define keys fot terminals
##

if [ $DISPLAY ]; then
    if [ "`uname`"=="FreeBSD" ]; then
        bindkeys_method=2 		# auto from termcap (work in FreeBSD)
    elif [ "`uname`"=="OpenBSD" ]; then
        bindkeys_method=2 		# auto from termcap (work in FreeBSD)
    else
        bindkeys_method=3 		# "suse" method
    fi
else
    if [ "`uname`"=="Linux" ]; then
	bindkeys_method=3 
    else
	bindkeys_method=1 			# define zkdb
    fi
fi

#PROXY="http://10.31.80.15:3128"

##
## Manual set method for bind keys
##
#bindkeys_method=2

# Const. sessings
source $HOME/.config/zshrc.inc
source /usr/share/wlib/tcolor_inc.sh

HISTSIZE=5000
SAVEHIST=5000

#proxyset $PROXY

export SHELL=/bin/zsh
export shell=/bin/zsh

ffile=`whereis most | cut -d' ' -f 2`
if [ -f $ffile ]; then export PAGER=most ; else export PAGER=less; fi

## Alias
alias utf-ru="export LANG=ru_RU.UTF-8 && export LC_ALL=ru_RU.UTF-8"
alias utf="utf-ru"

export EDITOR=mcedit

## locale
utf

if (( $SHLVL==1 )); then
  if sinsend ; then exit 0; fi
  echo ""
  echo " Вход в сессию пользователя LOGNAME: $LOGNAME"
  echo " Выход \"Ctrl-C\", \"exit\" [Enter]"
  echo " Отправить СИН: \"sendsin\""
  echo " Полная Справка: \"aspd help\""
  echo ""
fi
