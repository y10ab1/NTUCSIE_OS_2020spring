#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "sudo this."
	exit 1
fi

in_folder="../OS_PJ1_Test"

dmesg -C

for i in FIFO_1 FIFO_2 FIFO_3 FIFO_4 FIFO_5 PSJF_1 PSJF_2 PSJF_3 PSJF_4 PSJF_5 RR_1 RR_2 RR_3 RR_4 RR_5 SJF_1 SJF_2 SJF_3 SJF_4 SJF_5 TIME_MEASUREMENT;
do
	echo Output to $i ...
	echo ""
	../main < $in_folder/$i.txt > ../output/$i"_stdout.txt"
	echo ""
	echo Deleting previous dmesg ...
	echo ""
	echo Output $i dmesg ...
	dmesg -c | grep project1 > ../output/$i"_dmesg.txt"
	echo ""
done
	echo done! 

