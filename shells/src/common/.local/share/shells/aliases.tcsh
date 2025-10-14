############################################################################
##
## aliases.tcsh
##
## wocson (c) 2008 BSD
##
############################################################################

alias proxy "source $LIBINCPATH/tcshrc.d/proxy.tcsh"

set aliascmd = (`grep 'alias' $LIBINCPATH/share/aliases.sh | grep -v '#' | tr " " "@" | tr '\n' ' '`)

foreach cmd ( ${aliascmd} )
    set cmd = `echo "$cmd" | tr '@' ' ' | tr '=' ' ' | tr "'" " "`
    set cmd = "${cmd:gas/export/setenv/}"
    $cmd
end
