##############################################################################
##
## tcshrc.inc
##
## wocson (c) 2008 BSD
##
##############################################################################

## PROMPT
if ($?prompt) then
    # An interactive shell -- set some stuff up

    set color_name = '%{^[[ color_code m%}'
    set rgb_restore   = '%{^[[00m%}'
    set rgb_black     = '%{^[[00;30m%}'
    set rgb_firebrick = '%{^[[00;31m%}'
    set rgb_red       = '%{^[[01;31m%}'
    set rgb_forest    = '%{^[[00;32m%}'
    set rgb_green     = '%{^[[01;32m%}'
    set rgb_brown     = '%{^[[00;33m%}'
    set rgb_yellow    = '%{^[[01;33m%}'
    set rgb_navy      = '%{^[[00;34m%}'
    set rgb_blue      = '%{^[[01;34m%}'
    set rgb_purple    = '%{^[[00;35m%}'
    set rgb_magenta   = '%{^[[01;35m%}'
    set rgb_cadet     = '%{^[[00;36m%}'
    set rgb_cyan      = '%{^[[01;36m%}'
    set rgb_gray      = '%{^[[00;37m%}'
    set rgb_white     = '%{^[[01;37m%}'
    set rgb_std       = "${rgb_white}"

    if ( ${?root} ) then
        set rgb_usr = "${rgb_red}"
        set rgb_delim = "${rgb_red}"
    else
        set rgb_usr = "${rgb_white}"
        set rgb_delim = "${rgb_green}"
    endif

    set prompt="${rgb_yellow}[${rgb_green}%T ${rgb_usr}`whoami`${rgb_yellow}@${rgb_green}%M ${rgb_white}%c${rgb_yellow}][${rgb_green}tcsh ${rgb_yellow}#${rgb_green}$SHLVL${rgb_yellow}]\n${rgb_delim}%#${rgb_restore} "

    # Clean mc-trash from history
    sed '/^ *$/d; / *printf*/d; / *set*/d; / *cd*/d' $HOME/.history > ./tmpf && mv -f ./tmpf $HOME/.history

    ######### tune tcsh ############################

    set nobeep
    set correct = cmd # cmd - only command, complete - do common symbol, all - cmd+path
    set autolist = ambiguous
    set ignoreeof
    set autocorrect
    set complete = enhance

    set history = 5000
    set savehist = 5000
    set histdup = erase # all, prev, erase
#    set histcontrol = ignoreboth
#    set savedirs
    set filec

    set autorehash
    set autoexpand
    set rmstar
    set printexitvalue
    set dextract

    set color
    set colorcat
    set noding

    set watch = (0 any any)
#    set who = "%n(`whoami`) has %a %l from %M"
    unset who    

    ################################################

    set mail = (/var/mail/$USER)

    if ($?tcsh) then
	    bindkey "^W" backward-delete-word
  	    bindkey -k up history-search-backward
  	    bindkey -k down history-search-forward
    endif

endif # if for prompt

## SETTINGS for X and console

if ($?DISPLAY) then
    if (-f $HOME/.Xresources ) then
        xrdb $HOME/.Xresources
    endif
endif

source $LIBINCPATH/aliases.tcsh
