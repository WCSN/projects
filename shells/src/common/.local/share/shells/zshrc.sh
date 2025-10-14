##############################################################################
##
## zshrc.inc
##
## wocson (c) 2008 BSD
##
##############################################################################

# Custom completion support via /etc/zsh_completion.d

#fpath=( $fpath /etc/zsh_completion.d )

## Define keys fot terminals
if [ $DISPLAY ]; then
    if [ "`uname`"=="FreeBSD" ]; then
        bindkeys_method=2         # auto from termcap (work in FreeBSD)
    elif [ "`uname`"=="OpenBSD" ]; then
        bindkeys_method=2         # auto from termcap (work in FreeBSD)
    else
        bindkeys_method=3         # "suse" method
    fi
else
    if [ "`uname`"=="Linux" ]; then
        bindkeys_method=3
    else
        bindkeys_method=1             # define zkdb
    fi
fi

export BINDKEYS=$bindkeys_method

if [[ "$ZSHEDIT" == "vi" ]]; then
    bindkey    -v
else
    bindkey    -e
    bindkey    "^[ "    magic-space
    bindkey    "^[!"    expand-history
fi

autoload -U colors && colors
autoload -Uz compinit && compinit

function prmt_length()
{
    emulate -L zsh
    local COLUMNS=${2:-$COLUMNS}
    local -i x y=${#1} m
    if (( y )); then
        while (( ${${(%):-$1%$y(l.1.0)}[-1]} )); do
            x=y
            (( y *= 2 ))
        done
        while (( y > x + 1 )); do
            (( m = x + (y - x) / 2 ))
            (( ${${(%):-$1%$m(l.x.y)}[-1]} = m ))
        done
    fi
    echo $x
}

autoload -Uz vcs_info

zstyle ':vcs_info:*' enable svn git
zstyle ':vcs_info:*' check-for-changes true
zstyle ':vcs_info:*' get-revision true
zstyle ':vcs_info:*' stagedstr "+"
zstyle ':vcs_info:*' unstagedstr "!"
#zstyle ':vcs_info:*' formats "%F{gray}%s: %F{reset}%r/%S%F{gray}%} %F{green}%b%F{reset}%m%u%c%F{reset}"
zstyle ':vcs_info:*' formats "%F{gray}%s: %F{green}%b%F{reset} %B%F{green}%m%F{red}%u%F{cyan}%c%F{reset}"
# %s    Используемые vcsы (git, hg, svn и т.д.).
# %b    Информация о текущей ветке.
# %a    Идентификатор, описывающий действие. Имеет смысл только в actionformats.
# %i    Текущий номер редакции или идентификатор. Для настройки выходных данных в hg может использоваться стиль hgrevformat.
# %c    Строка из стиля stagedstr, если в репозитории есть поэтапные изменения.
# %u    Строка из стиля unstagedstr, если в репозитории есть нестационарные изменения.
# %R    Базовый каталог репозитория.
# %r    Имя репозитория. Если %R - это /foo/bar/repoXY, то %r - это repoXY.
# %S    Подкаталог в репозитории. Если $PWD - это /foo/bar/repoXY/beer/tasty, то %S - это beer/tasty.
# %m    Это замена "разное". На усмотрение серверной части, чтобы решить, на что распространяется эта замена.
setopt PROMPT_SUBST

precmd()
{
    vcs_info
    VCS=" %b\${vcs_info_msg_0_}%b"
    [ "$UID" = "0" ] && USR="red" || USR="gray"; [ "$UID" = "0" ] && PRM="red" || PRM="green";
    if [[ -z $CONNC ]]; then
        UL1="%B%F{yellow}[%F{green}%T %F{$USR}%n%F{yellow}@%F{green}%M%F{gray} %~%F{yellow}]"
    else
        UL1="%B%F{yellow}[%F{green}%T %F{$USR}%n%F{yellow}@%F{green}%M%F{gray}:${CONNC} %~%F{yellow}]"
    fi
    iZ1="[%F{green}zsh%F{yellow} #%F{green}$SHLVL%F{yellow}]%K{reset}%F{reset}"
    UL=${UL1}${iZ1}
    DL="%B%F{$PRM}%#%b%K{reset}%F{reset}%b "
    let sX=$(tput cols);
    let szVCS=$(prmt_length "$VCS");
    let szUL=sX-szVCS
    NL=$'\n'    # New line
    UL=$(printf "%-${szUL}s%s" "${UL}" "${VCS}")
    PROMPT="${UL}${NL}${DL}"
}

# Set/unset  shell options
setopt pushdtohome
setopt autolist
setopt nopromptcr
setopt autocd
setopt recexact
setopt longlistjobs
setopt pushdsilent
setopt autopushd
setopt pushdminus
setopt extendedglob
#setopt noglob
setopt rcquotes
setopt correctall
setopt menucomplete
zstyle ':completion:*' menu select=1 _complete _ignored _expand _correct _approximate
setopt IGNORE_EOF
setopt NO_BEEP
unsetopt bgnice
unsetopt autoparamslash
unsetopt hup

##
## History
##
setopt append_history
setopt hist_expire_dups_first
setopt hist_find_no_dups
setopt HIST_IGNORE_ALL_DUPS
setopt HIST_IGNORE_SPACE
setopt HIST_REDUCE_BLANKS
setopt share_history
setopt extended_history

HISTFILE=$HOME/.zsh_history

if [ -f $HOME/.zhistory ]; then
    mv -f $HOME/.zhistory $HISTFILE
elif [ ! -f $HISTFILE ]; then
    # copy from tcsh-history
    sed '/ *#/d; /^ *$/d; / *printf*/d; / *set*/d; / *cd*/d' $HOME/.history >> $HISTFILE
fi

compinit

# Completion so "cd ..<TAB>" -> "cd ../"
# zstyle ':completion:*' special-dirs ..
# Online help
unalias  run-help 2>/dev/null || true
autoload run-help

# Don't use zsh builtin which

alias which >/dev/null && unalias which

function suse_method()
{
    # Common standard keypad and cursor
    bindkey    "^[[2~"        overwrite-mode
    bindkey    "^[[3~"        delete-char

    # History completion on pgup and pgdown
    autoload -U history-search-end
    zle -N history-beginning-search-backward-end history-search-end
    zle -N history-beginning-search-forward-end history-search-end
    bindkey "^[[5~" up-line-or-history
    bindkey "^[[6~" down-line-or-history

    if [[ "$TERM" == "xterm" || "$TERM" == "xterm-256color" ]]; then
        bindkey    "^[2;5~"        overwrite-mode
        bindkey    "^[3;5~"        delete-char
        bindkey    "^[5;5~"        up-history
        bindkey    "^[6;5~"        down-history
    fi

    bindkey    "^[[C"        forward-char
    bindkey    "^[[D"        backward-char
    bindkey    "^[[A"        history-beginning-search-backward
    bindkey    "^[[B"        history-beginning-search-forward
    # Avoid network problems
    #   ... \177 (ASCII-DEL) and \010 (ASCII-BS)
    #       do `backward-delete-char'
    # Note: `delete-char' is maped to \033[3~
    #       Therefore xterm's responce on pressing
    #       key Delete or KP-Delete should be
    #       \033[3~ ... NOT \177
    #'
    bindkey    "^?"        backward-delete-char
    bindkey    "^H"        backward-delete-char

    # Home and End
    if [[ "$TERM" == "xterm" || "$TERM" == "xterm-256color" ]]; then
        # Normal keypad and cursor of xterm
        bindkey    "^[[1~"    history-search-backward
        bindkey    "^[[4~"    set-mark-command
        bindkey    "^[[H"    beginning-of-line
        bindkey    "^[[F"    end-of-line

        # Home and End of application keypad and cursor of xterm
        bindkey    "^[OH"    beginning-of-line
        bindkey    "^[OF"    end-of-line
        bindkey    "^[O5H"    beginning-of-line
        bindkey    "^[O5F"    end-of-line
    else
        if [[ "$TERM" == "kvt" ]]; then
            bindkey    "^[[1~"    history-search-backward
            bindkey    "^[[4~"    set-mark-command
            bindkey    "^[OH"    beginning-of-line
            bindkey    "^[OF"    end-of-line
        else
            # TERM=linux or console
            bindkey    "^[[1~"    beginning-of-line
            bindkey    "^[[4~"    end-of-line
        fi
    fi

    # Application keypad and cursor of xterm
    if [[ "$TERM" == "xterm" || "$TERM" == "xterm-256color" ]]; then
        bindkey    "^[OD"    backward-char
        bindkey    "^[OC"    forward-char
        bindkey    "^[OA"    history-beginning-search-backward
        bindkey    "^[OB"    history-beginning-search-forward
        # DEC keyboard KP_F1 - KP_F4
        bindkey -s "^[OP"    "^["
        bindkey    "^[OQ"    undo
        bindkey    "^[OR"    undefined-key
        bindkey    "^[OS"    kill-line
    fi

    if [[ "$TERM" == "gnome" ]]; then
        # or gnome terminal F1 - F4
        bindkey -s "^[OP"    "^["
        bindkey    "^[OQ"    undo
        bindkey    "^[OR"    undefined-key
        bindkey    "^[OS"    kill-line
    fi

    # Function keys F1 - F12
    if [[ "$TERM" == "linux" ]]; then
        # On console the first five function keys
        bindkey    "^[[[A"    undefined-key
        bindkey    "^[[[B"    undefined-key
        bindkey    "^[[[C"    undefined-key
        bindkey    "^[[[D"    undefined-key
        bindkey    "^[[[E"    undefined-key
    else
        # The first five standard function keys
        bindkey    "^[[11~"    undefined-key
        bindkey    "^[[12~"    undefined-key
        bindkey    "^[[13~"    undefined-key
        bindkey    "^[[14~"    undefined-key
        bindkey    "^[[15~"    undefined-key
    fi

    bindkey    "^[[17~"        undefined-key
    bindkey    "^[[18~"        undefined-key
    bindkey    "^[[19~"        undefined-key
    bindkey    "^[[20~"        undefined-key
    bindkey    "^[[21~"        undefined-key

    # Note: F11, F12 are identical with Shift_F1 and Shift_F2
    bindkey    "^[[23~"        undefined-key
    bindkey    "^[[24~"        undefined-key
    # Shift Function keys F1  - F12
    #      identical with F11 - F22
    #
    # bindkey   "^[[23~"    undefined-key
    # bindkey   "^[[24~"    undefined-key
    bindkey    "^[[25~"        undefined-key
    bindkey    "^[[26~"        undefined-key
    # DEC keyboard: F15=^[[28~ is Help
    bindkey    "^[[28~"        undefined-key
    # DEC keyboard: F16=^[[29~ is Menu
    bindkey    "^[[29~"        undefined-key
    bindkey    "^[[31~"        undefined-key
    bindkey    "^[[32~"        undefined-key
    bindkey    "^[[33~"        undefined-key
    bindkey    "^[[34~"        undefined-key

    if [[ "$TERM" == "xterm" || "$TERM" == "xterm-256color" ]]; then
        # Not common
        bindkey    "^[[35~"    undefined-key
        bindkey    "^[[36~"    undefined-key
    fi

    if [[ "$TERM" == "xterm" || "$TERM" == "xterm-256color" ]]; then
        # Application keypad and cursor of xterm
        # with NumLock ON
        #
        # Operators
        bindkey -s "^[Oo"    "/"
        bindkey -s "^[Oj"    "*"
        bindkey -s "^[Om"    "-"
        bindkey -s "^[Ok"    "+"
        bindkey -s "^[Ol"    ","
        bindkey -s "^[OM"    "\n"
        bindkey -s "^[On"    "."
        # Numbers
        bindkey -s "^[Op"    "0"
        bindkey -s "^[Oq"    "1"
        bindkey -s "^[Or"    "2"
        bindkey -s "^[Os"    "3"
        bindkey -s "^[Ot"    "4"
        bindkey -s "^[Ou"    "5"
        bindkey -s "^[Ov"    "6"
        bindkey -s "^[Ow"    "7"
        bindkey -s "^[Ox"    "8"
        bindkey -s "^[Oy"    "9"
    fi

    #  EMACS line editing
    if [[ "$ZSHEDIT" == "emacs" ]]; then
        # ... xterm application cursor
        if [[ "$TERM" == "xterm" || "$TERM" == "xterm-256color" ]]; then
            bindkey    "^[^[OD"     backward-word
            bindkey    "^[^[OC"     forward-word
            bindkey    "^[^[OA"     history-beginning-search-backward
            bindkey    "^[^[OB"     history-beginning-search-forward
            bindkey    "^^[OD"      backward-char
            bindkey    "^^[OC"      forward-char
            bindkey    "^^[OA"      history-beginning-search-backward
            bindkey    "^^[OB"      history-beginning-search-forward
        fi

        # Standard cursor
        bindkey    "^[^[[D"    backward-word
        bindkey    "^[^[[C"    forward-word
        bindkey    "^[^[[A"    history-beginning-search-backward
        bindkey    "^[^[[B"    history-beginning-search-forward
        bindkey    "^^[[D"     backward-char
        bindkey    "^^[[C"     forward-char
        bindkey    "^^[[A"     history-beginning-search-backward
        bindkey    "^^[[B"     history-beginning-search-forward
    fi
}

function defkeys()
{
    # setup key accordingly
    [[ -n "${key[Home]}"     ]]  && bindkey  "${key[Home]}"      beginning-of-line
    [[ -n "${key[End]}"      ]]  && bindkey  "${key[End]}"       end-of-line
    [[ -n "${key[Insert]}"   ]]  && bindkey  "${key[Insert]}"    overwrite-mode
    [[ -n "${key[Delete]}"   ]]  && bindkey  "${key[Delete]}"    delete-char
    [[ -n "${key[Up]}"       ]]  && bindkey  "${key[Up]}"         history-beginning-search-backward # up-line-or-search
    [[ -n "${key[Down]}"     ]]  && bindkey  "${key[Down]}"      history-beginning-search-forward  # down-line-or-search
    [[ -n "${key[Left]}"     ]]  && bindkey  "${key[Left]}"      backward-char
    [[ -n "${key[Right]}"    ]]  && bindkey  "${key[Right]}"     forward-char
    [[ -n "${key[PageUp]}"   ]]  && bindkey  "${key[PageUp]}"    up-line-or-history
    [[ -n "${key[PageDown]}" ]]  && bindkey  "${key[PageDown]}"  down-line-or-history
}

# Finally, make sure the terminal is in application mode, when zle is
# active. Only then are the values from $terminfo valid.
function zle-line-init ()
{
    echoti smkx
}

function zle-line-finish ()
{
    echoti rmkx
}

function bind_keys()
{
    if [ "$1" = "1" ]; then
        autoload zkbd
        bindkeys_file=$HOME/.zkbd/$TERM-$VENDOR-$OSTYPE
        if [ ! -f $bindkeys_file ]; then
               zkbd
        fi

        source $bindkeys_file
        defkeys
    elif [ "$1" = "2" ]; then
        typeset -A key
        key[Home]=${terminfo[khome]}
        key[End]=${terminfo[kend]}
        key[Insert]=${terminfo[kich1]}
        key[Delete]=${terminfo[kdch1]}
        key[Up]=${terminfo[kcuu1]}
        key[Down]=${terminfo[kcud1]}
        key[Left]=${terminfo[kcub1]}
        key[Right]=${terminfo[kcuf1]}
        key[PageUp]=${terminfo[kpp]}
        key[PageDown]=${terminfo[knp]}
        defkeys
    elif [ "$1" = "3" ]; then
        typeset -g -A key
        suse_method
    else
        echo '' #default
    fi
}

bind_keys $bindkeys_method # Set in .zshrc

if [ -n "${DISPLAY}" ]; then
    zle -N zle-line-init
    zle -N zle-line-finish

    # colors for xterm
    if [ -f "./.Xresources" ]; then
        xrdb ./.Xresources
    fi
fi

source $LIBINCPATH/aliases.sh
