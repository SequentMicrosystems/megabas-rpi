import megabas as m
import time

print('We assume 0-10V output channels are connected to 0-10V input channels ')
for i in range(1,5):
	m.setUOut(0, 1, 0)
time.sleep(0.3)

for j in range(1, 5):
	print('0-10V output channel ' + str(j) + " to 0-10V input channel " + str(j))
	for i in range(10):
		m.setUOut(0, j, i)
		time.sleep(0.1)
		print(m.getUOut(0, j), m.getUIn(0, j))

for i in range(1,5):
	m.setUOut(0, 1, 0)
time.sleep(0.3)

for j in range(1, 5):
	print('0-10V output channel ' + str(j) + " to 0-10V input channel " + str(j+4))
	for i in range(10):
		m.setUOut(0, j, i)
		time.sleep(0.1)
		print(m.getUOut(0, j), m.getUIn(0, j+4))