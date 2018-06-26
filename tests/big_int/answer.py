for i in range (0, 100):
	f = open('test' + str(i) + '.in', 'r')
	g = open('test' + str(i) + '.out', 'w')
	numbers = []
	g.write('Enter two numbers n and m:\n')
	for line in f:
		numbers.append(int(line))
	n = numbers[0]
	m = numbers[1]
	g.write('n is ' + str(n) + '\n')
	g.write('m is ' + str(m) + '\n')
	g.write('n + m is ' + str(n + m) + '\n')
	g.write('n - m is ' + str(n - m) + '\n')
	g.write('n * m is ' + str(n * m) + '\n')
	if (m == 0):
		g.write('m is 0, division is undefined')
	else:
		quo = (abs(n) // abs(m))
	if (n * m < 0):
		quo = -quo
	g.write('n / m is ' + str(quo) + '\n')
	g.write('n % m is ' + str(n - quo * m) + '\n')
