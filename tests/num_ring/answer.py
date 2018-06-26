def exp(b, n, m):
	if (m == 0):
		return 1
	ind = 1
	now = n
	while (2 * ind <= m):
		ind *= 2
		now = now ** 2
		now %= b
	return (exp(b, n, (m - ind)) * now) % b


for i in range (0, 100):
	f = open('test' + str(i) + '.in', 'r')
	g = open('test' + str(i) + '.out', 'w')
	numbers = []
	g.write('Enter the number base b:\n')
	g.write('Enter two numbers n and m:\n')
	for line in f:
		numbers.append(int(line))
	b = numbers[0]
	n = numbers[1]
	m = numbers[2]
	b = abs(b)
	if (b == 0):
		g.write('b is 0')
	else:
		n = ((n % b) + b) % b
		m = ((m % b) + b) % b
		g.write('b is ' + str(b) + '\n')
		g.write('n mod b is ' + str(n) + '\n')
		g.write('m mod b is ' + str(m) + '\n')
		g.write('n + m is ' + str((((n + m) % b) + b) % b) + '\n')
		g.write('n - m is ' + str((((n - m) % b) + b) % b) + '\n')
		g.write('n * m is ' + str((((n * m) % b) + b) % b) + '\n')
		g.write('n ** m is ' + str(exp(b, n, m)) + '\n')
