##################################################
##
## proxyset.sh
## wocson (c)
##

function outinfo()
{
    echo " http_proxy: $http_proxy"
    echo "https_proxy: $https_proxy"
    echo "  ftp_proxy: $ftp_proxy"
    echo " HTTP_PROXY: $HTTP_PROXY"
    echo "HTTPS_PROXY: $HTTPS_PROXY"
    echo "  FTP_PROXY: $FTP_PROXY"
    echo "      PROXY: $PROXY"
}

function setproxy()
{
    local PRX="$1"

    export http_proxy="http://$PRX"
    export https_proxy="https://$PRX"
    export ftp_proxy="ftp://$PRX"
    
    export HTTP_PROXY=$http_proxy
    export HTTPS_PROXY=$https_proxy
    export FTP_PROXY=$ftp_proxy
}

function unsetproxy()
{
    unset http_proxy
    unset https_proxy
    unset ftp_proxy

    unset HTTP_PROXY
    unset HTTPS_PROXY
    unset FTP_PROXY
}

function warnerr()
{
    echo "Proxy not set! Set env PROXY or use proxy set"  
}

function usage()
{
    echo "proxy [set|unset] [IP:PORT]"
    echo "proxy [on|off]"
}

function proxy()
{
    local CMD="$1" Proxy="$2"

    if [[ -z "$PROXY" ]] && [[ -z "$Proxy" ]]; then 
        warnerr 
        usage
        return
     fi

    case "$CMD" in
    set)
        if [[ -n "$PROXY" ]] && [[ -z "$Proxy" ]]; then 
            Proxy="$PROXY"
        fi
        setproxy "$Proxy" 
        export PROXY="$Proxy"
        outinfo
    ;;
    unset)
        unsetproxy
        unset PROXY
    ;;
    on)
        if [[ -z "$PROXY" ]]; then
            usage
            warnerr
        else
            setproxy "$PROXY" 
        fi
    ;;
    off)
        unsetproxy
    ;;
    view)
        if [[ -z "$PROXY" ]]; then 
            warnerr 
            usage
        else
            outinfo
        fi
    ;;
    help|usage)
        usage        
    ;;
    *)
        if [[ -z "$PROXY" ]]; then 
            warnerr 
            usage
        else
            outinfo
        fi
    ;;
    esac
}

unpack() 
{
    if [ -f $1 ] ; then
        # display usage if no parameters given
        echo "Usage: unpack [path/file_name].[tar.7z|zip|rar|bz2|gz|tar|tbz2|tgz|Z|7z|xz|ex|tar.bz2|tar.gz|tar.xz]"
        case $1 in
            *.tar.bz2)   tar xvjf "$1"    ;;
            *.tar.gz)    tar xvzf "$1"    ;;
            *.tar.xz)    tar xvJf "$1"    ;;
            *.tar.7z)    7z x -so "$1" | tar xf - -C ./ ;; 
            *.7z)        7z x "$1"        ;;
            *.lzma)      unlzma "$1"      ;;
            *.bz2)       bunzip2 "$1"     ;;
            *.rar)       rar x "$1"       ;;
            *.gz)        gunzip "$1"      ;;
            *.tar)       tar xvf "$1"     ;;
            *.tbz2)      tar xvjf "$1"    ;;
            *.tgz)       tar xvzf "$1"    ;;
            *.zip)       unzip "$1"       ;;
            *.xz)        unxz "$1"        ;;
            *.Z)         uncompress "$1"  ;;
            *)           echo "don't know how to unpack '$1'..." ;;
        esac
    else
        echo "'$1' is not a valid file!"
    fi
}
