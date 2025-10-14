##############################################################################
##
## proxy.tcsh
##
## wocson (c) 2008 BSD
##
##############################################################################

alias outinfo "source $LIBINCPATH/tcshrc.d/prx_outinfo.tcsh"
alias setproxy "source $LIBINCPATH/tcshrc.d/prx_setproxy.tcsh"
alias unsetproxy "source $LIBINCPATH/tcshrc.d/prx_unsetproxy.tcsh"
alias usage "source $LIBINCPATH/tcshrc.d/prx_usage.tcsh"
alias warnerr "source $LIBINCPATH/tcshrc.d/prx_warnerr.tcsh"

set argc = $#
set CMD = ""
#set Proxy = ""

if ( $argc >= 1 ) then
    set CMD="$argv[1]" 
endif

if ( $argc >= 2 ) then
    set Proxy="$argv[2]"
endif

if ( ! $?PROXY  && ! $?Proxy ) then 
    warnerr
    usage
    exit
endif

switch ( $CMD )
case "set":
    if ( $?PROXY && ! $?Proxy ) then 
        set Proxy="$PROXY"
    endif
    setproxy "$Proxy" 
    setenv PROXY "$Proxy"
    outinfo
breaksw
case "unset":
    unsetproxy
    unsetenv PROXY
breaksw
case "on":
    if ( ! $?PROXY ) then
        warnerr
        usage
    else
        setproxy "$PROXY" 
    endif
breaksw
case "off":
    unsetproxy
breaksw
case "view":
    if ( ! $?PROXY ) then 
        warnerr 
        usage 
    else
        outinfo
    endif
breaksw
case "help","usage":
    usage        
breaksw
default:
    if ( ! $?PROXY ) then 
        warnerr 
        usage 
    else
        outinfo
    endif
endsw
