import matplotlib.pyplot as plt
import time
import serial

def graphData():
	#data order: rightSense, leftSense, diffSense, deriv, error, rightSpeed, leftSpeed
	graph_data = []
	with open('data.txt') as f:
		data = f.readlines()
	for s in data:
		tmp_data = [float(d) for d in s.strip().split(',')]
		graph_data.append(tmp_data)

	


def gatherData():
	"""Collects data from serial output of Arduino and writes it to a text file called
	'data.txt'. Runs until user stops output."""

	port = '/dev/ttyUSB0'									# Arduino Port Number
	frequency = '9600'										# Frequency

	ser = serial.Serial(port,frequency,timeout=2)
	time.sleep(2)

	f = open('data.txt','w')

	while True:
		if ser.inWaiting():
			try:
				rawData = ser.readline()
				f.write(rawData)
				time.sleep(0.2)
			except:
				time.sleep(0.2)

	#f.write(rawData)
	f.close()

if __name__ == '__main__':
	graphData()
