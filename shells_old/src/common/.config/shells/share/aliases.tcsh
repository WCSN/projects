############################################################################
##
## proxyset.sh
## wocson (c)
##
## Aliases
##
## This aliases for tcsh translate from bash-shell aliases.sh to tcsh-shell 
##
############################################################################

alias proxy "source $LIBINCPATH/tcshrc.d/proxy.tcsh"

set aliascmd = (`grep 'alias' $LIBINCPATH/share/aliases.sh | grep -v '#' | tr " " "@" | tr '\n' ' '`)

foreach cmd ( ${aliascmd} )
    set cmd = `echo "$cmd" | tr '@' ' ' | tr '=' ' ' | tr "'" " "`
    set cmd = "${cmd:gas/export/setenv/}"
    $cmd
end
