import matplotlib 
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import scipy.optimize as optimization
from mpl_toolkits.mplot3d import Axes3D

data_array = []

x_array = []
y_array = []
z_array = []


result_file = r'results.dat'
with open(result_file) as file:
    data_array = [[float(digit) for digit in line.split()] for line in file]
    
def old_data():
    for i in range (0,len(data_array)):
        x_array.append(data_array [i][0])
        y_array.append(data_array [i][1])
        z_array.append(data_array [i][2])       


old_data()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(x_array, y_array, z_array, c='r')


ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

plt.show()

