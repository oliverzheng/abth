#!/bin/bash

rm -f percentages.txt

for ((i=1; i<=44; i+=1)); do
	echo "$i / 100"
	good=`grep "^[0-9]*:[0-9]*.*\<$i\>.*good" msnptraffic.txt | wc -l`
	awry=`grep "^[0-9]*:[0-9]*.*\<$i\>.*awry" msnptraffic.txt | wc -l`
	if [ $good -ne 0 ] || [ $awry -ne 0 ]; then
		total=$(($good + $awry))
		pr=`echo $awry | awk "{print 100 - $awry / $total * 100}"`
		echo "$i	$pr" >> percentages.txt
	fi
done
