#!/bin/bash

MODULE_NAME="network_mod.ko"
INTERFACE_NAME="dummy0"
INTERFACE_IP="192.168.123.123"
INTERFACE_MAC="06:05:04:03:02:01"

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

echo -e "\n  Ifconfig $INTERFACE_NAME:\n"
ifconfig $INTERFACE_NAME

echo -e "\n  Set MAC address $INTERFACE_MAC for interface $INTERFACE_NAME\n"
ifconfig $INTERFACE_NAME hw ether $INTERFACE_MAC
echo -e "  Set IP address $INTERFACE_IP for interface $INTERFACE_NAME\n"
ifconfig $INTERFACE_NAME $INTERFACE_IP

echo -e "  Ifconfig $INTERFACE_NAME:\n"
ifconfig $INTERFACE_NAME

echo -e "  Ping interface:\n"
ping -c 4 $INTERFACE_IP

rmmod $MODULE_NAME

echo -e "\n  Log info:\n"
tail /var/log/kern.log

echo ""
