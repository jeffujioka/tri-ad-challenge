#!/bin/bash

if [ ! -d ./output/debug/test/report ]; then
	mkdir -p ./output/debug/test/report
fi

for f in ./output/debug/bin/utest-*
do
	BASENAME=$(basename $f)
	$f "--gtest_output=xml:./output/debug/test/report/$BASENAME.xml"
	if [ $? -ne 0 ]; then
		echo "Failed to run $BASENAME"
		exit 1
	fi
done
