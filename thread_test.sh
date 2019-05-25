#!/bin/bash

function printEcho {
	echo "================================================="
	echo "		$1"
	echo "================================================="
}

printEcho "echo-thread run"
./echo-thread &

printEcho "process status"
ps -ef | grep echo-thread | grep -v grep

printEcho "client1 connect"
telnet localhost 9999 &

wait $!

printEcho "process status"
ps -ef | grep echo-thread | grep -v grep

printEcho "clinet2 connect"
telnet localhost 9999 &

wait $!

printEcho "process status"
ps -ef | grep echo-thread | grep -v grep

printEcho "kill process"

ps -ef | grep echo-thread | grep -v grep | awk '{print $2}' | xargs kill
