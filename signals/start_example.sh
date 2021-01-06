#!/bin/bash

if ! test -f signals ; then
	echo "App not found"
	exit
fi

./signals &

sleep 0.1

echo "Send SIGUSR1"
killall -SIGUSR1 signals

sleep 0.5

echo "Send SIGUSR2"
killall -SIGUSR2 signals

sleep 0.5

echo "Send SIGTERM"
killall signals

sleep 0.5

echo "Send SIGKILL"
killall -9 signals
