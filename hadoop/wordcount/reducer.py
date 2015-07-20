#!/usr/bin/python
import sys

wordlist = {}

for line in sys.stdin:
	line = line.strip()
	word, count = line.split()
	word = word.lower()
	count = int(count)

	if word in wordlist:
		wordlist[word] += count
	else:
		wordlist[word] = count

for word, count in wordlist.iteritems():
	print word, count

