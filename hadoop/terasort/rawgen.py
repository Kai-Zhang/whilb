#!/usr/bin/python
import random

datafile = open('rawinput.data', 'w')
rawdata = random.sample(range(100000), 100000)

for data in rawdata:
	datafile.write(str(data) + '\n')

datafile.close()

