import matplotlib 
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import scipy.optimize as optimization

data_array = []

x_array = []
t_array = []

#x = np.array([0, 100, 20000, 30000, 10000, 4000, 5000, 6000, 7000, 8000, 9000])
#plt.gca().set_color_cycle(['red', 'blue', 'green', 'yellow', 'purple'])

result_file = r'results.dat'
with open(result_file) as file:
    data_array = [[float(digit) for digit in line.split()] for line in file]
    
def old_data():
    for i in range (0, len(data_array)):#x[j], x[j+1]):
        x_array.append(data_array [i][0])
        t_array.append(data_array [i][1])

for j in range (0, 1):
    old_data()
    plt.plot(x_array, t_array, 'o')

    x_array = []
    t_array = []



#plt.legend([ "Ground state, $E_0$", "$E_1$", "$E_2$", "L = 100", "L = 90", "L = 80", "L = 60", "Cold start beta = 0.48"], 
#           bbox_to_anchor=[0.25, 1.1])
plt.title ("$\omega$ vs Step Size")
plt.xlabel("$\omega$")
plt.ylabel("Time Step")

plt.show()

