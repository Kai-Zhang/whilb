import random

dataset = random.sample(range(-100, 100), len(range(-100, 100)))

output = open('sortdata', 'w')

for item in dataset:
	output.write(str(item) + '\n')

