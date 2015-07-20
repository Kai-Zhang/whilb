#!/usr/bin/python
import sys
import bisect

labels = []

labelfile = open('label.data', 'r')
for line in labelfile:
	labels.append(line.strip())

for line in sys.stdin:
	line = line.strip()
	key = line.split()[0]
	pos = bisect.bisect_left(labels, key)
	if pos == len(labels): pos -= 1
	pos = pos * 20 / len(labels)
	print '%d %s\t%s' % (pos, key, line)

