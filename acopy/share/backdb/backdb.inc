#!/bin/bash
#
# Backup copy to USB drive
#
# (c) Wocson 2012
#
# Create: 20121108
#
#

clean_b()
{
  rm -fv $1
  touch $1
}

message()
{
  ii=1

  for i in "$@"
    do
      if [ $ii -gt 1 ]; then
      echo -e `date "+%Y%m%d.%H%M"` "$1: `hostname -s` ${i}" >> $REPFILE
      logg "$1: `hostname -s` ${i}"
      fi
      ii=$(( $ii + 1 ))
  done
}

logg()
{
  echo -e `date "+%Y%m%d.%H%M"` $1 >> $LOGF
  echo -e `date "+%Y%m%d.%H%M"` $1
}

back_cp()
{
  message "INFO" " from $1 to $2/$3"
  
  if [ -d "$2" ] ; then
    cp -fv $1 $2/$3
  else
    mkdir -p $2
    cp -fv $1 $2/$3
  fi

#  message "INFO" "Contents: $2" `ls -1  $2`
}
