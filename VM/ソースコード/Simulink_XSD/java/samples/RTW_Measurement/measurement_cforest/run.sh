#!/bin/sh

cforest_wait=5

cforest_log=cforest.log

./cforest.sh > $cforest_log &

cforest_pid=$!

sleep $cforest_wait

if kill -0 $cforest_pid; then
	./gdb.sh
	gdb_status=$?
	if test $gdb_status -ne 0; then
		echo "GDB exit status: $gdb_status" 1>&2
		kill $cforest_pid
		exit 1
	fi
else
	echo "CForest not running" 1>&2
	exit 1
fi

count=0
cforest_done=n

while test $count -lt 3
do
	if kill -0 $cforest_pid 2>/dev/null; then
		sleep 1;
	else
		cforest_done=y
		break;
	fi
done

if test $cforest_done = y; then
    sed -n \
	-e '/^---- BEGIN MEASUREMENT ----/,/^---- END MEASUREMENT ----/p' \
	$cforest_log | sed '/^---- /d' > result.csv
fi
