#!/usr/bin/python
import sys

elements = []

for line in sys.stdin:
	items = line.strip().split()
	items = [ int(e) for e in items ]
	elements += items

#elements.sort()
for e in elements:
	print str(e) + '\t1'
print ''
