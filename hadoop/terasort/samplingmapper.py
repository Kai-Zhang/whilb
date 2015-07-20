#!/usr/bin/python
import sys
import random

keys = []

for line in sys.stdin:
	try: keys.append(line.strip().split()[0])
	except: continue

if len(keys) > 1: size = 1
else: size = len(keys)

indexes = random.sample(range(len(keys)), size)
for index in indexes:
	print str(keys[index]) + '\t1'

