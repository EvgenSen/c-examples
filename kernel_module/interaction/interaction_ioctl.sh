#!/bin/bash

MODULE_NAME="interaction_ioctl.ko"
DEV_NAME="interaction_ioctl"

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

mknod /dev/$DEV_NAME c 100 0

echo -e "\n  Start userspace_ioctl:\n"
./userspace_ioctl 7 test_msg_for_kernel

rmmod $MODULE_NAME

rm /dev/$DEV_NAME

echo -e "\n  Log info:\n"
tail /var/log/kern.log

echo ""
