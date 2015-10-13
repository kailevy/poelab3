import matplotlib.pyplot as plt
import time
import serial

def graphData():
	#data order: rightSense, leftSense, diffSense, deriv, error, rightSpeed, leftSpeed
	graph_data = {'right_sense': [],
	'left_sense': [],
	'diff_sense': [],
	'deriv': [],
	'error': [],
	'right_speed': [],
	'left_speed': [],
	'time': []}
	with open('data.txt') as f:
		data = f.readlines()
	for index, s in enumerate(data):
		tmp_data = [float(d) for d in s.strip().split(',')]
		graph_data['right_sense'].append(tmp_data[0])
		graph_data['left_sense'].append(tmp_data[1])
		graph_data['diff_sense'].append(tmp_data[2])
		graph_data['deriv'].append(tmp_data[3])
		graph_data['error'].append(tmp_data[4])
		graph_data['right_speed'].append(tmp_data[5])
		graph_data['left_speed'].append(tmp_data[6])
		graph_data['time'].append(index * 0.1)
		
	#GRAPH FOR IF SENSORS
	# plt.plot(graph_data['time'], graph_data['diff_sense'])
	# plt.plot(graph_data['time'], graph_data['right_sense'])
	# plt.plot(graph_data['time'],graph_data['left_sense'])
	# plt.legend(["Difference between sensors", "Right sensor", "Left sensor"], loc ="center")
	# plt.title("IR Reflectance Sensor Data over One Lap Time")
	# plt.xlabel("Time (seconds)")
	# plt.ylabel("Reading")
	# plt.show()

	#GRAPH FOR MOTOR SPEEDS
	# plt.plot(graph_data['time'], graph_data['right_speed'])
	# plt.plot(graph_data['time'], graph_data["left_speed"])
	# plt.legend([ "Right motor speed", "Left motor speed"])
	# plt.title("Left and Right Motor Speeds over Time")
	# plt.xlabel("Time (seconds)")
	# plt.ylabel("Speed")
	# plt.ylim([0,60])
	# plt.show()

	#GRAPH FOR INTEGRAL
	# plt.plot(graph_data['time'], graph_data['error'])
	# plt.title("Sensor Difference Integral over One Lap Time")
	# plt.xlabel("Time (seconds)")
	# plt.ylabel("Integral")
	# plt.show()

	#GRAPH FOR DERIVATIVE
	# plt.plot(graph_data['time'], graph_data['deriv'])
	# plt.title("Sensor Difference Derivative over One Lap Time")
	# plt.xlabel("Time (seconds)")
	# plt.ylabel("Derivative")
	# plt.show()


	
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
