#!/usr/bin/python

data = []
fileindex = range(20)

for index in fileindex:
	thisfile = open('sorted/part-%05d' % index, 'r')
	for line in thisfile:
		data.append(line.strip())
	thisfile.close()

raw = data[:]
data.sort()

if raw == data:
	print 'Valid'
else:
	print 'Failed'

