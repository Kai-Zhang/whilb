#!/usr/bin/python
import sys

arrays = []
curlist = []

for line in sys.stdin:
	line = line.strip()
	if line == '':
		arrays.append(curlist)
		curlist = []
	else:
		value, key = line.split('\t')
		try: curlist.append(int(value))
		except: continue

if curlist != []:
	arrays.append(curlist)

def merge(a, b):
	if (len(a) * len(b) == 0): return a+b
	if (a[0] > b[0]): a, b = b, a
	return [a[0]] + merge(a[1:], b)

rst = reduce(merge, arrays)

for e in rst:
	print e

