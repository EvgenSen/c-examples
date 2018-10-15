#!/bin/bash

if [[ $(id -u) != "0" ]]; then
	echo "Permission denied (you must be root)"
	exit
fi

if ! test -f hello_world.ko ; then
	echo "Module not found"
	exit
fi

echo -e "\n  Module info:\n"
modinfo hello_world.ko

insmod hello_world.ko

rmmod hello_world.ko

echo -e "\n  Log info:\n"
tail /var/log/kern.log

echo ""
