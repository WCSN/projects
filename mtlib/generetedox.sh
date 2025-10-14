#!/bin/sh
ROOT_DIR=$1
cd $ROOT_DIR
/usr/local/doxygen/bin/doxygen ./doxy.conf > /dev/null  2>&1
PROJECT_NAME=`cat ./doxy.conf | grep PROJECT_NAME | awk -F"=" '{print $2}' | sed 's/^ //'`
#echo $PROJECT_NAME
P_DIR="/usr/local/apache_php4/htdocs/doxygen/$PROJECT_NAME"
if [ ! -d $P_DIR ]
then
mkdir /usr/local/apache_php4/htdocs/doxygen/$PROJECT_NAME
fi

cp -r ./docs/html/* /usr/local/apache_php4/htdocs/doxygen/$PROJECT_NAME

echo "For list doxigen data -> http://10.255.2.99/doxygen/$PROJECT_NAME"
