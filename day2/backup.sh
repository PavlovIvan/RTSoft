#!/bin/bash

s_dirs=""
s_mask=""

while [ "$#" != 0 ] && [ "$1" != "--mask" ] && [ "$1" != "--dirs" ];
do
	shift
done

while [ "$#" != 0 ];
do
	if [ "$1" == "--dirs" ]; 
	then
		shift
		#echo "dirs:"
		while [ "$1" != "--mask" ] && [ "$#" != 0 ];	
		do
			#echo "$1"
			s_dirs="$s_dirs $1"
			#echo "$s_dirs" 
			shift
		done
	fi
	if [ "$1" == "--mask" ]; 
	then
		shift
		#echo "mask:"
		while [ "$1" != "--dirs" ] && [ "$#" != 0 ];	
		do
			#echo "$1"
			s_mask="$s_mask $1"
			shift
		done
	fi
done
for cur_mask in $s_mask;
do
	find $s_dirs -name $cur_mask -exec cp "{}" ./my_backup \;
done
