import matplotlib 
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import scipy.optimize as optimization
from scipy.stats import binned_statistic



data_array = []

energy_array = []
magnetization_array = []
timestep_array = []
temperature_array = []


autocorrelation_function_array = []
t_array = []

expectation_value_mag = 0
expectation_value_energy = 0

O = 0
O_expect = 0
numerator = 0
denominator = 0
variance = 0

N = 10000  ##lattice size CHANGE APPROPRIATELY


result_file = r'netmag.dat'
with open(result_file) as file:
    data_array = [[float(digit) for digit in line.split()] for line in file]
    
def write_to_file(x,y,data_file):

    f = open(data_file,'a')
    
    f.write('%f' % x)
    f.write('\t')
    f.write('%f' % y)
    f.write('\n')
    f.close

    
def data():
    for i in range (thermalization_value, t_max):
        energy_array.append(data_array [i][1])
        magnetization_array.append(data_array [i][0])
        timestep_array.append(data_array [i][2])
        temperature_array.append(data_array [i][3])


t_max = 1000000
thermalization_value = 0

data()

beta = temperature_array [10]


for i in range (thermalization_value,  t_max):
    O_expect  += magnetization_array[i]   #calculate expectation value

O_expect /= (t_max - thermalization_value) #O_expect

#We calculate the autocorrelation times for each t   
for t in range (0, 100, 1): #

    for i in range (thermalization_value,  t_max - t):
        O_t1 = magnetization_array[i]
        O_t1_t = magnetization_array[i+t]
        variance_t1 = O_t1 - O_expect
        variance_t1_t = O_t1_t - O_expect
        numerator += variance_t1*variance_t1_t
        
        
    numerator /= (t_max - t - thermalization_value)         ###gives the entire numerator

    for i in range (thermalization_value,  t_max):
        O = magnetization_array[i]
        variance = O - O_expect
        variance = variance**2
        denominator += variance

    denominator /= (t_max - thermalization_value)

    autocorrelation_function = numerator/denominator



    print "Autocorrelation Function:", autocorrelation_function
    print "beta:" , beta
    print "t:", t

    autocorrelation_function_array.append(autocorrelation_function)    
    t_array.append (t)

    write_to_file(autocorrelation_function, t, 'autocorrelations.dat')
                  
    O = 0
    numerator = 0
    denominator = 0

energy_array = []
magnetization_array = []
timestep_array = []
temperature_array = []

    
#plt.semilogy(t_array, autocorrelation_function_array, 'o')
plt.plot (t_array, autocorrelation_function_array)
#plt.yscale ('log')
plt.xlabel("t")
plt.ylabel("p_o(t)")
plt.show()


