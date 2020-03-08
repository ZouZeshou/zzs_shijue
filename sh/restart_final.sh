#!/bin/sh
while true
do
	ps -ef | grep "RM2020_Infantry_vision" | grep -v "grep"
	if [ "$?" -eq 1 ]
	then
			/home/ubuntu/ZZS_vision/StartVision.sh
			echo "process has been restarted!"
	else
			echo "process already started!"
	fi
	sleep 1
done
