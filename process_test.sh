#!/bin/bash

function printEcho {
	echo "===================================================="
	echo "		$1"
	echo "===================================================="
}

./echo-process &

if [ $? -eq 0 ]; then
	printEcho "echo-process run"
else
	printEcho "echo-process fail"
fi

printEcho "process status"
ps -ef | grep echo-process | grep -v grep

printEcho "client1 connect"
telnet localhost 9999 &

wait $!

printEcho "process status"
ps -ef | grep echo-process | grep -v grep

printEcho "client2 connect"
telnet localhost 9999 &

wait $!

printEcho "process status"
ps -ef | grep echo-process | grep -v grep

printEcho "kill process"

ps -ef | grep echo-process | grep -v grep | awk '{print $2}' | xargs kill
