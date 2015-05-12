#!/bin/csh

if [ $HDMI ] then
	echo "lol";
	unset $HDMI;
else
	echo "set HDMI";
	set $HDMI;
fi;
