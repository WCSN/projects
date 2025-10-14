#!/bin/sh
#sed -n 's/.*\/\(\(D_\|R_\)[0-9_]*\)\/.*/\1/p'
cd ../
VERSION=`cat .svn/entries | grep m.sources | head -n 1 | sed -n 's/.*\/\(\(D_\|R_\)[0-9_]*\)[^0-9]*/\1/p'`
echo $VERSION > build/version.tag

