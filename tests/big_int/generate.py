from random import randint
suite_file = 'suite.txt'
g = open(suite_file, 'w')
for i in range (0, 100):
	g.write('test'+str(i) + '\n')
	file_name = 'test' + str(i) + '.in'
	f = open(file_name, 'w')
	n = randint(-(10**(3 * i + 1)), 10**(3 * i + 1))
	m = randint(-(10**(3 * i)), 10**(3 * i))
	f.write(str(n) + '\n')
	f.write(str(m) + '\n')
