#!/bin/bash

PROC_NAME="inter_mod_proc"

if [[ $(id -u) != "0" ]]; then
	echo "Permission denied (you must be root)"
	exit
fi

if ! test -f interaction_proc.ko ; then
	echo "Module not found"
	exit
fi

echo -e "\n  Module info:\n"
modinfo interaction_proc.ko

insmod interaction_proc.ko

echo -e "\n  Proc output:\n"
cat /proc/$PROC_NAME

echo "test msg for kernel" > /proc/$PROC_NAME

rmmod interaction_proc.ko

echo -e "\n  Log info:\n"
tail /var/log/kern.log

echo ""
