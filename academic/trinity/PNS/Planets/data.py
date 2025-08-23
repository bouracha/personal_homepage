import matplotlib 
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import scipy.optimize as optimization

data_array = []

x0_array = []
y0_array = []
x1_array = []
y1_array = []
x2_array = []
y2_array = []
x3_array = []
y3_array = []

x = np.array([0, 100, 20000, 30000, 40000, 4000, 5000, 6000, 7000, 8000, 9000])

result_file = r'planets.dat'
with open(result_file) as file:
    data_array = [[float(digit) for digit in line.split()] for line in file]
    
def old_data():
    for i in range (0, len(data_array)):# x[j], x[j+1]):
        x0_array.append(data_array [i][0])
        y0_array.append(data_array [i][1])
        x1_array.append(data_array [i][2])
        y1_array.append(data_array [i][3])
        x2_array.append(data_array [i][4])
        y2_array.append(data_array [i][5])
        x3_array.append(data_array [i][6])
        y3_array.append(data_array [i][7])
        
for j in range (0, 4):
    old_data()
    plt.plot(x0_array, y0_array, 'r-')
    plt.plot(x1_array, y1_array, 'b-')
    plt.plot(x2_array, y2_array, 'g-')
    plt.plot(x3_array, y3_array, 'y-')





plt.xlabel("x")
plt.ylabel("y")

plt.show()

