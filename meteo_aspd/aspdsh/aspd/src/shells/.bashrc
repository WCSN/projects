##############################################
##
## .bashrc 
## (c) wocson
##

test -s ~/.alias && . ~/.alias || true

#PROXY="http://10.31.80.15:3128"

source $HOME/.config/bashrc.inc

#export EDITOR=/usr/bin/vim
export EDITOR=mcedit

ffile=`whereis most | cut -d' ' -f 2`
if [ -f $ffile ]; then export PAGER=most ; else export PAGER=less; fi

export PKG_FETCH="/usr/local/bin/wget -O '%2\$s' '%1\$s'"
export SHELL=/bin/bash
export shell=/bin/bash

#proxyset $PROXY
#export NEWSSERVER=your.news.server

alias cp866="export LANG=ru_RU.CP866 && export LC_ALL=ru_RU.CP866"                                                              
alias utf="export LANG=ru_RU.UTF-8 && export LC_ALL=ru_RU.UTF-8"   

alias pfu="portsnap fetch update"
alias spa='df -h | grep "100%" | grep -v "/dev" | grep -v "proc"'

alias utf-en="export LANG=en_US.UTF-8 && export LC_ALL=en_US.UTF-8"
alias utf-ru="export LANG=ru_RU.UTF-8 && export LC_ALL=ru_RU.UTF-8"
alias iso-en="export LANG=en_US.ISO8859-1 && export LC_ALL=en_US.ISO8859-1"
alias cp866="export LANG=ru_RU.CP866 && export LC_ALL=ru_RU.CP866"
alias utf="utf-ru"
alias sus="sudo -s"
alias me="mcedit"

# locale 
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
