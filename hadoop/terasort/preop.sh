#!/bin/sh
set -x
${HADOOP_HOME}/bin/hadoop dfs -get /user/rd/label.data label.data
if [ $? -ne 0 ]; then
	echo "download fail" >&2
	exit -1
fi
python sortmapper.py
if [ $? -ne 0 ]; then
	echo 'Mapper fail' >&2
	exit -1
fi
rm -f labels.data

