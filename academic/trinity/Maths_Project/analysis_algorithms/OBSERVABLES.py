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

beta_array = []

energy_array = []
magnetization_array = []
timestep_array = []
temperature_array = []

binned_array_mag = []
binned_array_energy = []
bin_array_heatcapacity = []  ###
bin_array_susceptibility = []  ###

error_mag_array = []
error_energy_array = []
error_susceptibility_array = []
error_heatcapacity_array = []
expectmag_array = []
expectenergy_array = []
expectsusceptibility_array = []   ###
expectheatcapacity_array = []   ###

suceptibility_array = []
specificheat_array = []

bin_size_array = []
bin_number_array = []

expectation_value_mag = 0
expectation_value_energy = 0
expectation_value_heatcapacity = 0   ###
expectation_value_susceptibility = 0 ###

O_expect_mag = 0
O_expect_energy = 0
O_expect_hea = 0    ###
O_expect_hea_err = 0    ###
O_expect_sus = 0    ###
O_expect_sus_err = 0    ###

O_mag = 0
O_energy = 0
O_hea = 0 ###
O_hea_err = 0 ###
O_sus = 0 ###
O_sus_err = 0 ###

error_mag = 0
error_energy = 0

N = 40000  ##lattice size


result_file = r'netmag.dat'
with open(result_file) as file:
    data_array = [[float(digit) for digit in line.split()] for line in file]

def write_to_file(x,y,z,p,t,c,d,e,g,data_file):

    f = open(data_file,'a')
    
    f.write('%f' % x)
    f.write('\t')
    f.write('%f' % y)
    f.write('\t')
    f.write('%f' % z)
    f.write('\t')
    f.write('%f' % p)
    f.write('\t')
    f.write('%f' % t)
    f.write('\t')
    f.write('%f' % c)
    f.write('\t')
    f.write('%f' % d)
    f.write('\t')
    f.write('%f' % c)
    f.write('\t')
    f.write('%f' % g)
    f.write('\n')
    f.close   

def data():
    for i in range (min_value_of_interest, max_value_of_interest):
        energy_array.append(data_array [i][1])
        magnetization_array.append(data_array [i][0])
        timestep_array.append(data_array [i][2])
        temperature_array.append(data_array [i][3])

timestep_number = 50000
number_of_points = 1
b = 10000 #bin number
thermalization_value = 10000

for x in range (0, number_of_points):

    min_value_of_interest = x*timestep_number
    max_value_of_interest = min_value_of_interest + timestep_number 

    data()
    beta = temperature_array[10]

#################################################################
    #mag full error and expectation value and susceptibility
############################################################
    for i in range (thermalization_value, timestep_number, b):
        bin_average = 0
        counter = 0
        O_expect_sus = 0    ###
        O_sus = 0     ###
        for j in range (i, i+b):
            if j == timestep_number:
                break
            bin_average += magnetization_array[j]
            counter +=1
################################################################
            #Here we shall calculate heat capacity in each bin.
            #Cal a C for each bin then the error and expectation value, considering each bin as one point
            O_expect_sus += magnetization_array[j]   ###
            O_sus += ((magnetization_array[j])**2)  ###
        O_expect_sus /= float(b)    ###
        O_expect_sus = O_expect_sus**2    ###
        O_sus /= float(b)    ###

        susceptibility = (beta*N*(O_sus - O_expect_sus))   ###
        #print "susceptibility per bin:", susceptibility

#########################################################
        if counter == b:     ###Check if bins are full!
            bin_average = bin_average/float(b)
            binned_array_mag.append(bin_average)
            bin_array_susceptibility.append(susceptibility)    ###
            expectation_value_susceptibility += susceptibility   ###
################################################################
            O_expect_sus_err += susceptibility     ###
            O_sus_err += ((susceptibility)**2)    ###
################################################################
    expectation_value_susceptibility /= float(len(bin_array_susceptibility))   ###
    O_expect_sus_err /= float(len(bin_array_susceptibility))                      ###
    O_expect_sus_err = O_expect_sus**2    ### 
    O_sus_err /= float(len(bin_array_susceptibility))      ###

    error_susceptibility = np.sqrt(((O_sus - O_expect_sus)/(len(bin_array_susceptibility)-1)))   ###



    for i in range (0, len(binned_array_mag)):
        expectation_value_mag  += binned_array_mag [i]

    expectation_value_mag /= float(len(binned_array_mag))

    for i in range (0, len(binned_array_mag)):
        O_expect_mag  += binned_array_mag[i]
        O_mag = O_mag + ((binned_array_mag[i])**2)

    O_expect_mag /= len(binned_array_mag)
    O_expect_mag = O_expect_mag**2
    O_mag = O_mag/len(binned_array_mag)



    susceptibility2 = (beta*N*(O_mag - O_expect_mag))
    error_mag = np.sqrt(((O_mag - O_expect_mag)/(len(binned_array_mag)-1)))
#####################################################################################
#####################################################################################

#################################################################
    #energy full error and expectation value and heat capacity
############################################################
    for i in range (thermalization_value, timestep_number, b):
        bin_average = 0
        counter = 0
        O_expect_hea = 0    ###
        O_hea = 0     ###
        for j in range (i, i+b):
            if j == timestep_number:
                break
            bin_average += energy_array[j]
            counter +=1
################################################################
            #Here we shall calculate heat capacity in each bin.
            #Cal a C for each bin then the error and expectation value, considering each bin as one point
            O_expect_hea += energy_array[j]   ###
            O_hea += ((energy_array[j])**2)  ###
        O_expect_hea /= b    ###
        O_expect_hea = O_expect_hea**2    ###
        O_hea /= b    ###

        heat_capacity = (beta*beta*N*(O_hea - O_expect_hea))   ###


        if counter == b:
            bin_average = bin_average/float(b)
            binned_array_energy.append(bin_average)
            bin_array_heatcapacity.append(heat_capacity)    ###
            expectation_value_heatcapacity += heat_capacity   ###
################################################################
            O_expect_hea_err += heat_capacity     ###
            O_hea_err += ((heat_capacity)**2)    ###
################################################################

    expectation_value_heatcapacity /= float(len(binned_array_energy))    ###
    O_expect_hea_err /= float(len(binned_array_energy))                       ###
    O_expect_hea_err = O_expect_hea**2    ### 
    O_hea_err /= float(len(binned_array_energy))     ###

    error_heatcapacity = np.sqrt(((O_hea - O_expect_hea)/(len(binned_array_energy)-1)))   ###


    for i in range (0, len(binned_array_energy)):
        expectation_value_energy  += binned_array_energy [i]

    expectation_value_energy /= float(len(binned_array_energy))

    for i in range (0, len(binned_array_energy)):
        O_expect_energy  += binned_array_energy[i]
        O_energy = O_energy + ((binned_array_energy[i])**2)

    O_expect_energy /= len(binned_array_energy)
    O_expect_energy = O_expect_energy**2
    O_energy /= len(binned_array_energy)

    error_energy = np.sqrt(((O_energy - O_expect_energy)/(len(binned_array_energy)-1)))
##############################################################################
    
    #print "Size of bins:", b
    #print "Number of bins:", len(binned_array_mag)
    #print "<e>:", expectation_value_energy
    print "<c>:", heat_capacity
    #print "<m>:", expectation_value_mag
    print "<X>:", expectation_value_susceptibility 
    #print "Error_mag:", error_mag
    #print "Error_energy:", error_energy
    print "Error_susceptibility", error_susceptibility
    print "Error_heatcapacity", error_heatcapacity
    print "beta:" ,beta

    write_to_file(expectation_value_mag, expectation_value_energy, expectation_value_susceptibility, expectation_value_heatcapacity, error_mag, error_energy, error_susceptibility, error_heatcapacity, beta, 'results.dat')    

    error_mag_array.append (error_mag)
    error_energy_array.append (error_energy)
    error_susceptibility_array.append (error_susceptibility)
    error_heatcapacity_array.append (error_heatcapacity)
    expectmag_array.append (expectation_value_mag)
    expectenergy_array.append (expectation_value_energy)
    expectsusceptibility_array.append(expectation_value_susceptibility)
    expectheatcapacity_array.append(expectation_value_heatcapacity)
    bin_size_array.append(b)
    beta_array.append(beta)
    #size_array.append (n)
    #bin_number_array.append(n_b)

    expectation_value_energy = 0
    expectation_value_mag = 0
    expectation_value_heatcapacity = 0  ###
    expectation_value_susceptibility = 0  ###
    
    O_expect_mag = 0
    O_expect_energy = 0
    O_mag = 0
    O_energy = 0

    O_expect_hea_err = 0    ###
    O_expect_sus_err = 0    ###

    O_hea_err = 0 ###
    O_sus_err = 0 ###

    energy_array = []
    magnetization_array = []
    timestep_array = []
    temperature_array = []

    binned_array_mag = []
    binned_array_energy = []
    bin_array_heatcapacity = [] ###
    bin_array_susceptibility = []  ###

plt.plot (beta_array, expectsusceptibility_array, 'ro')
plt.errorbar (beta_array, expectsusceptibility_array, error_susceptibility_array, linestyle="o")
plt.yscale('log')
#plt.plot (beta_array, expectmag_array)
#plt.errorbar (beta_array, expectmag_array, error_mag_array, linestyle="o")
plt.xlabel("Beta")
plt.ylabel("<X>")
#plt.show()


