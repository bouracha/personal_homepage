import matplotlib 
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import scipy.optimize as optimization

def write_to_file(x,y,z,p,c,d,t,data_file):

    f = open(data_file,'a')
    
    f.write('%f' % x)
    f.write('\t')
    f.write('%f' % y)
    f.write('\t')
    f.write('%f' % z)
    f.write('\t')
    f.write('%f' % p)
    f.write('\t')
    f.write('%f' % c)
    f.write('\t')
    f.write('%f' % d)
    f.write('\t')
    f.write('%f' % t)
    f.write('\n')
    
    f.close
    
e1 = 2.71828182845904523536028747135266249775724709369995
#beta
array = [[-1 for x in range(3)] for x in range(3)]
beta = 0

energy_array = []
magnetization_array = []
beta_array = []

beta = 0.0

for temp in range (1, 50):

    sum_of_states_top = 0
    sum_of_states_bottom = 0
    sum_of_states_top_m = 0
    sum_of_states_bottom_m = 0
    beta += 0.02
    for a in range(-1, 2, 2):
        for b in range(-1, 2, 2):
            for c in range(-1, 2, 2):
                for d in range(-1, 2, 2):
                    for e in range(-1, 2, 2):
                        for f in range(-1, 2, 2):
                            for g in range(-1, 2, 2):
                                for h in range(-1, 2, 2):
                                    for i in range(-1, 2, 2):
                                        array [0][0] = a
                                        array [0][1] = b
                                        array [0][2] = c
                                        array [1][0] = d
                                        array [1][1] = e
                                        array [1][2] = f
                                        array [2][0] = g
                                        array [2][1] = h
                                        array [2][2] = i

                                        magnetization = 0
                                        total_energy = 0
                                        
                                        for n in range (0,3):
                                            for m in range (0,3):
                                                if n==0 and m == 0:
                                                    energy = (array[n][m])*(array[n+1][m]) + (array[n][m])*(array[n][m+1])
                                                    total_energy += energy
                                                elif n==2 and m == 2:
                                                    energy = (array[n][m])*(array[0][m]) + (array[n][m])*(array[n][0])
                                                    total_energy += energy
                                                elif n==0 and m == 1:
                                                    energy = (array[n][m])*(array[n+1][m]) + (array[n][m])*(array[n][m+1])
                                                    total_energy += energy
                                                elif n == 1 and m == 1:
                                                    energy = (array[n][m])*(array[n+1][m]) + (array[n][m])*(array[n][m+1])
                                                    total_energy += energy
                                                elif n==2 and m == 1:
                                                    energy = (array[n][m])*(array[0][m]) + (array[n][m])*(array[n][m+1])
                                                    total_energy += energy
                                                elif n==0 and m == 2:
                                                    energy = (array[n][m])*(array[n+1][m]) + (array[n][m])*(array[n][0])
                                                    total_energy += energy
                                                elif n==1 and m == 2:
                                                    energy = (array[n][m])*(array[n+1][m]) + (array[n][m])*(array[n][0])
                                                    total_energy += energy
                                                elif n==1 and m == 0:
                                                    energy = (array[n][m])*(array[n+1][m]) + (array[n][m])*(array[n][m+1])
                                                    total_energy += energy
                                                elif n==2 and m == 0:
                                                    energy = (array[n][m])*(array[0][m]) + (array[n][m])*(array[n][m+1])
                                                    total_energy += energy

                                                else:
                                                    print "Out of range:", n , m
                                                
                                                magnetization += (array[n][m])

                                        magnetization = abs(magnetization)
                                        #print magnetization
                                        total_energy = -total_energy

                                        top_m  = (magnetization)*(e1)**(-(beta)*total_energy)
                                        bottom_m  = (e1)**(-(beta)*total_energy)

                                        top = (total_energy)*(e1)**(-(beta)*total_energy)
                                        bottom = (e1)**(-(beta)*total_energy)



                                        sum_of_states_top += top
                                        sum_of_states_bottom += bottom

                                        sum_of_states_top_m += top_m
                                        sum_of_states_bottom_m += bottom_m

    

    #print sum_of_states_top
    print "Partition Function:", sum_of_states_bottom
    expectation_value_Energy = (sum_of_states_top/sum_of_states_bottom)/18
    expectation_value_Magnetization = ((sum_of_states_top_m/sum_of_states_bottom_m)/9)

    energy_array.append (expectation_value_Energy)
    magnetization_array.append (expectation_value_Magnetization)
    beta_array.append (beta)

    write_to_file(expectation_value_Magnetization, expectation_value_Energy, expectation_value_Magnetization, expectation_value_Energy, expectation_value_Energy,expectation_value_Energy, beta, 'exact_results.dat')
    
    print  "For beta =", beta
    print "<E> = ",   expectation_value_Energy                                         
    print "<M> = ",   abs(expectation_value_Magnetization)

plt.plot (beta_array, magnetization_array)
plt.xlabel("Beta")
plt.ylabel("Energy per site")
plt.show()
                                                

