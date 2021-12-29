#!/bin/bash

MODULE_NAME="timer_base.ko"

if [[ $(id -u) != "0" ]]; then
	echo "Permission denied (you must be root)"
	exit
fi

if ! test -f $MODULE_NAME ; then
	echo "Module $MODULE_NAME not found"
	exit
fi

echo -e "\n  Module info:\n"
modinfo $MODULE_NAME

insmod $MODULE_NAME

sleep 1

rmmod $MODULE_NAME

echo -e "\n  Log info:\n"
tail -20 /var/log/kern.log

echo ""
