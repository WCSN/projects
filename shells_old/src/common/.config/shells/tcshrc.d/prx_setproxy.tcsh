##################################################
##
## tcsh proxyset.tcsh
## wocson (c) 2024
##
##################################################
set argc = $#

if ( $argc >= 1 ) then

    set Proxy="$argv[1]"

    setenv http_proxy  "http://$Proxy"
    setenv https_proxy "https://$Proxy"
    setenv ftp_proxy   "ftp://$Proxy"
    setenv HTTP_PROXY  "http://$Proxy"
    setenv HTTPS_PROXY "https://$Proxy"
    setenv FTP_PROXY  "ftp://$Proxy"

endif
