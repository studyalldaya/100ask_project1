#!/bin/sh
#copy this file to $PATH
status=$1
if [ "$status" = "ok" ]
then
echo "$0 has been tested, it is ok"
fi

if [ "$status" = "err" ]
then
echo "$0 has been tested, it is err"
fi
