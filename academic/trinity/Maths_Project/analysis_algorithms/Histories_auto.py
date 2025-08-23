import matplotlib 
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import scipy.optimize as optimization

data_array = []

autocorrelation_array = []
timestep_array = []


result_file = r'autocorrelations.dat'
with open(result_file) as file:
    data_array = [[float(digit) for digit in line.split()] for line in file]
    
#This array gives us a convenient way of overplotting data of different lengths
x = np.array([0, 100, 100, 150, 200, 101, 6000, 7000, 8000, 9000])
plt.gca().set_color_cycle(['red', 'blue', 'green', 'yellow', 'purple'])



#j=number of different sets of data we want to overplot
for j in range (0, 1):
    counter = 0
    tau = 0
    tau_i = 0
    for i in range (x[j], x[j+1]):
        autocorrelation_array.append(data_array [i][0])
        timestep_array.append(data_array [i][1])
        #Calculate tau for autocorrelation function between 1 and 0.14
        if (1> data_array [i][0] > 0.14):
            counter += 1
            y = np.log(data_array [i][0])
            y = -(data_array [i][1])/y
            tau += y
            tau_i += y**2
    tau /= counter
    #plt.semilogy(timestep_array, autocorrelation_array, 'o', markersize = 5)
    plt.plot(timestep_array, autocorrelation_array, '-o', markersize = 3)
    
    print "tau:", tau
        
    tau_expect = tau**2
    tau_i /= counter
    
    error_tau = np.sqrt(((tau_i - tau_expect)/(counter-1)))

    print "Error_tau:", error_tau

    if (j==0):
        x_min=tau# - error_tau
        x_max=tau + error_tau
    if (j==1):
        y_min=tau# - error_tau
        y_max=tau + error_tau

    autocorrelation_array = [] 
    timestep_array = []

times = 2
#print "z = " , np.log(x_min/y_min)/np.log(times)
#print "z_error = " , abs(np.log(x_min/y_min)/np.log(times) - np.log(x_max/y_max)/np.log(times))


plt.legend([ "L = 100", "L = 50", "L = 20", "L = 10", "L = 90", "L = 80", "L = 60", "Cold start beta = 0.48"], 
           bbox_to_anchor=[1., 1])
plt.title(r'')
plt.xlabel(r'# of Timesteps (t)')
plt.ylabel(r'p_O(t)')
plt.show()

