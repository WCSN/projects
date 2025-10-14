##############################################################################
##
## bashrc.sh
##
## wocson (c) 2008 BSD
##
##############################################################################

prompt_command()
{
    # set an error string for the prompt, if applicable
    if [ $? -eq 0 ]; then
        ERRPROMPT=" "
    else
        ERRPROMPT=" ($?) "
    fi

    local TIME=$( date +%l:%M | cut -f2 -d' ' )
    local CURENT_PATH=$(echo ${PWD/#$HOME/\~})

    # trim long path
    if [ ${#CURENT_PATH} -gt "35" ]; then
        let CUT=${#CURENT_PATH}-35
        CURENT_PATH="...$(echo -n $PWD | sed -e "s/\(^.\{$CUT\}\)\(.*\)/\2/")"
    fi

    local GREEN="\[\033[0;32m\]"
    local CYAN="\[\033[0;36m\]"
    local BCYAN="\[\033[1;36m\]"
    local BLUE="\[\033[0;34m\]"
    local YELLOW="\[\033[0;33m\]"
    local GRAY="\[\033[0;37m\]"
    local DKGRAY="\[\033[1;30m\]"
    local WHITE="\[\033[1;37m\]"
    local RED="\[\033[0;31m\]"

    local LTGREEN="\[\033[0;1;32m\]"
    local LTCYAN="\[\033[0;1;36m\]"
    local LTBLUE="\[\033[0;1;34m\]"
    local LTYELLOW="\[\033[0;1;33m\]"
    local LTGRAY="\[\033[0;1;37m\]"
    local LTDKGRAY="\[\033[1;1;30m\]"
    local LTRED="\[\033[0;1;31m\]"

    # return color to Terminal setting for text color
    local DEFAULT="\[\033[0;39m\]"

    # different prompt and color for root
    local PR="${LTGREEN}$ "
    local USERNAME_COLORED="${WHITE}${USER}${LTYELLOW}@${LTGREEN}${HOSTNAME}"
    if [ "$UID" = "0" ]; then
        PR="${LTRED}# "
        USERNAME_COLORED="${LTRED}${USER}${LTYELLOW}@${LTGREEN}${HOSTNAME}"
    fi

    local TOP_LINE="${LTYELLOW}[${LTGREEN}${TIME} ${USERNAME_COLORED} ${LTGRAY}${CURENT_PATH}${LTYELLOW}][${LTGREEN}bash ${LTYELLOW}#${LTGREEN}${SHLVL}${LTYELLOW}]"
    local BOTTOM_LINE="${LTGREEN}${BRANCH}${PR}${DEFAULT}"
    export PS1="${TOP_LINE}\n${BOTTOM_LINE}"
}

if [ -n "${DISPLAY}" ]; then

    # colors for xterm
    if [ -f "./.Xresources" ]; then
        xrdb ./.Xresources
    fi
fi

PROMPT_COMMAND=prompt_command

source $LIBINCPATH/aliases.sh
