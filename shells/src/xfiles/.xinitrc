# Setup some extra PATH variables
PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/games:/usr/local/sbin:/usr/local/bin:$HOME/bin; export PATH

#XDG_DATA_DIRS="/share:/usr/share:/usr/local/share"
#export XDG_DATA_DIRS

#XDG_CONFIG_DIRS="/usr/local/etc/xdg"
#export XDG_CONFIG_DIRS

# Fix an issue with large image rendering via mozilla
#export MOZ_DISABLE_IMAGE_OPTIMIZE=1

    # colors for xterm
if [ -r ./.Xresources ]; then
    xrdb ./.Xresources
fi

