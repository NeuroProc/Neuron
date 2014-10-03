#!/bin/csh

for i in `find ./ -name "*.core"`
do 
	echo $i;
	rm $i;
done;
