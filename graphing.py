import matplotlib.pyplot as plt
import time
import serial

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