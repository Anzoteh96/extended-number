from random import randint

for i in range (0, 100):
	file_name = 'test' + str(i) + '.in'
	f = open(file_name, 'w')
	n = randint(-(10**(3 * i + 1)), 10**(3 * i + 1))
	m = randint(-(10**(3 * i)), 10**(3 * i))
	f.write(str(n) + '\n')
	f.write(str(m) + '\n')
