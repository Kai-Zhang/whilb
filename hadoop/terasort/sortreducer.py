#!/usr/bin/python
import sys

for line in sys.stdin:
	value = reduce(list.__add__, line.strip().split()[2:])
	print value

