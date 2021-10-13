#!/bin/sh

romPath=`dirname "$1"`
romName=`basename "$1"`

cd `dirname $0`

if [ ! -f $HOME/.scummvmrc ] ; then
	cp ./scummvmrc $HOME/.scummvmrc
fi

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

if [ ! -f $HOME/.debug_scummvmrc ] ; then
	exec ./scummvm -f -p"$romPath" ${romName%%.*} 2>&1 >/var/tmp/scummvm.log
else
	exec ./scummvm -d 11 -f -p"$romPath" ${romName%%.*} 2>&1 >/var/tmp/scummvm.log
fi

