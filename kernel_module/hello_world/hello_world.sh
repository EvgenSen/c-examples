#!/bin/bash

MODULE_NAME="hello_world.ko"

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

rmmod $MODULE_NAME

echo -e "\n  Log info:\n"
tail /var/log/kern.log

echo ""
