########################################################

import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
import math 
 
###Parameters###
 
#PI
pi=3.141592653589793238462
 
#Initialise rattler count
rattlers = 0
 
#Constant for harmonic repulsion ### Not necessary as force get non-dimensionalised
h_const = 100000
 
#Bin size for histogram
bin_size = 0.01
radii_bin_size = 0.0005
 

    
###Functions###
 
#Function for calulating overlaps in periodic packing
def overlap (x1, y1, r1, x2, y2, r2):
     
    distance = np.sqrt( (x1 - x2)**2 + (y1 - y2)**2)
     
    #Recalculate overlaps at boundary
    if (distance > r1 + r2):
 
        if abs(x1 - x2) > (r1 + r2):
            newdistance = np.sqrt((1 - abs(x1 - x2))**2 + (y1 - y2)**2)
 
        if abs(y2 - y1) > (r1 + r2):
            newdistance = np.sqrt((x1 - x2)**2 + (1 - abs(y1 - y2))**2)
 
        if abs(y2 - y1) > (r1 + r2) and abs(x1 - x2) > (r1 + r2):
            newdistance = np.sqrt((1 - abs(x1 - x2))**2 + (1 - abs(y1 - y2))**2)        
 
        delta = (r1 + r2) - newdistance
 
    else:    
        delta = (r1 + r2) - distance    
        
    return delta
 
 
##Function to put data x and data y in file data_file 
def write_to_file(y,x,data_file):

    f = open(data_file,'w')

    for i in range(0,len(x)):
        f.write('%f' % x[i])
        f.write('\t')
        f.write('%f' % y[i])
        f.write('\n')

    f.close()

def open_file(data_file):
    with open(data_file) as file:
        array = [[float(digit) for digit in line.split()] for line in file]
    return array
 
 
###Main###

for dataset in range (1,5):

    #Initialise arrays 
    norm_forc_dens = []
    norm_pres_dens = []
    final_radii_dens = []
    final_force_data = []    #Arrays to make the standard deviation of force calcuable
    final_radii = []        #Arrays to make the standard deviation of radii calcuable
    contact_number = []


    print 'Data set number:', dataset
    
    #Loop through files 
    for directory in range(1,2):

        print 'File number:', directory    

        if (dataset == 1):
            contact_file= r'C:\Users\Anthony\Desktop\Physics Project\readme\0'  + str(directory) + '\contacts_with_labels.dat'
            coordinate_file = r'C:\Users\Anthony\Desktop\Physics Project\readme\0'  + str(directory) + '\iterate.dat'
        if (dataset == 2):
            contact_file= r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-29\\'  + str(directory) + '\contacts.dat'
            coordinate_file = r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-29\\'  + str(directory) + '\Final.dat'
        if (dataset == 3):
            contact_file= r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-25\\'  + str(directory) + '\contacts.dat'
            coordinate_file = r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-25\\'  + str(directory) + '\Final.dat'
        if (dataset == 4):
            contact_file= r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-14\\'  + str(directory) + '\contacts.dat'
            coordinate_file = r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-14\\'  + str(directory) + '\Final.dat'

        #Import Data
        with open(contact_file) as file:
            arraycon = [[int(digit) for digit in line.split()] for line in file]
             
        with open(coordinate_file) as file:
            arraypos = [[float(digit) for digit in line.split()] for line in file]
             
     
        #Initialise array specific to each data set ##initialised in loop because must be set back to zero for each run of loop 
        force_data = []
        pressure_data = []
        radii = []
     
        total_balls = len(arraypos)
     
        #Loop over every sphere
        for i in range (0, total_balls - 1):     
     
            ball_i = i
            number_of_contacts = arraycon[ball_i][0]
            force_on_ball_i = []
     
            x1 = arraypos[ball_i][1]     
            y1 = arraypos[ball_i][2]
            if (dataset == 1):
                r1 = arraypos[ball_i][3]
            else:
                r1 = arraypos[ball_i][4]    #changed + 1 for poly since there is a z coordinate = 0
     
            radii.append(r1)
     
            if (number_of_contacts != 0):
     
                contact_number.append(number_of_contacts)
     
                #Loop over overlapping spheres
                if(dataset == 1):
                    startloop = 2
                    stoploop = number_of_contacts + 1
                else:
                    startloop = 1
                    stoploop = number_of_contacts
                    
                for j in range (startloop, stoploop ):       #changed -1, -1 for poly
     
                    ball_j = arraycon[ball_i][j] - 1
     
                    x2 = arraypos[ball_j][1]
                    y2 = arraypos[ball_j][2]
                    if (dataset == 1):
                        r2 = arraypos[ball_j][3]
                    else:
                        r2 = arraypos[ball_j][4]    #changed + 1 for poly since there is a z coordinate = 0
     
                    #Overlap calculation
                    delta = overlap (x1, y1, r1, x2, y2, r2)
     
                    #Force
                    force = h_const * delta
                    force_data.append(force)
     
                    #Pressure
                    force_on_ball_i.append(force)
                 
                pressure = sum(force_on_ball_i) / ( 2 * pi * r1 )
                pressure_data.append(pressure)
                 
            else:
                #count rattlers
                rattlers += 1
     
        mean_radii = np.mean(radii)
        ###Normalising Data###
     
        #Non-dimensionalise force and pressure
        force_data = force_data / np.mean(force_data)
        pressure_data = pressure_data / np.mean(pressure_data)
        radii = radii / mean_radii
        
        #Generate histograms  ###  Fixed the range because I couldn't add arrays of different lengths
        forc_x_axis = np.arange(0, 5, bin_size)
        pres_x_axis = np.arange(0, 5, bin_size) 
        radii_x_axis = np.arange(0, 2.5, bin_size) ##-Again fixed because can't add arrays of different lengths
     
        forc_dens , forc_bin_edges = np.histogram(force_data,    bins = forc_x_axis)
        pres_dens , pres_bin_edges = np.histogram(pressure_data, bins = pres_x_axis)
        radii_dens , radii_bin_edges = np.histogram(radii, bins = radii_x_axis)
     
     
        #Normalise histograms
        forc_normalising_factor = sp.simps(forc_dens, dx = bin_size)
        pres_normalising_factor = sp.simps(pres_dens, dx = bin_size)
     
        if len(norm_forc_dens) == 0:
            norm_forc_dens = forc_dens / forc_normalising_factor
        else:
            norm_forc_dens = norm_forc_dens + forc_dens / forc_normalising_factor
             
        if len(norm_pres_dens) == 0:
            norm_pres_dens = pres_dens / pres_normalising_factor
        else:
            norm_pres_dens = norm_pres_dens + pres_dens / pres_normalising_factor
            
        if len(final_radii_dens) == 0:
            final_radii_dens = radii_dens
        else:
            final_radii_dens = final_radii_dens + radii_dens 
            
        if len(final_radii) == 0:
            final_radii = radii
        else:
            np.append(final_radii, radii)          #This adds to the end of the array instead of adding the value of the points.

        if len(final_force_data) == 0:
            final_force_data = force_data
        else:
            np.append(final_force_data, force_data)          #This adds to the end of the array instead of adding the value of the points. 
        


        
    ###Plot histograms###
    forc_bin_centres = (forc_bin_edges[:-1] + forc_bin_edges[1:])/2
    pres_bin_centres = (pres_bin_edges[:-1] + pres_bin_edges[1:])/2
    radii_bin_centres = (radii_bin_edges[:-1] + radii_bin_edges[1:])/2
     
    norm_forc_dens = norm_forc_dens  / sp.simps(norm_forc_dens, dx = bin_size)
    norm_pres_dens = norm_pres_dens  / sp.simps(norm_pres_dens, dx = bin_size)
     
     
    #write_to_file(forc_bin_centres, norm_forc_dens, 'forc_dens.dat')

    write_to_file(forc_bin_centres, norm_forc_dens, 'forc_'+str(dataset)+'dens.dat') 
    write_to_file(pres_bin_centres, norm_pres_dens, 'pres_'+str(dataset)+'dens.dat')
     
    plt.title('G_14')
    plt.text(60,.025,r'$\mu=100,\ \sigma=0.14')
    plt.xlabel(r'$\tilde{r}=\frac{r}{\hat{r}}$',fontsize=16)
    plt.ylabel(r'Probability Density $P(\tilde{r})$')
    plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
    plt.plot(radii_bin_centres, final_radii_dens, 'gD', markersize = 3)
    plt.savefig('probdens_radii_14.png')
    plt.clf() 


    plt.xlabel(r'$\tilde{f}=\frac{f}{\hat{f}}$',fontsize=16)
    plt.ylabel(r'Probability Density $P(\tilde{f})$')
    plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
    plt.plot(forc_bin_centres, norm_forc_dens, 'D', markersize=3)
    plt.savefig('probdens_forc.png')    
    plt.clf()
     
    plt.xlabel(r'$\tilde{p}=\frac{p}{\hat{p}}$',fontsize=16)
    plt.ylabel(r'Probability Density $P(\tilde{p})$')
    plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
    plt.plot(pres_bin_centres, norm_pres_dens, 'rD', markersize=3)
    plt.plot(forc_bin_centres, norm_forc_dens, 'D', markersize=3)         
    plt.savefig('probdens_pres.png')    
    plt.clf()
     
    #Plot semi-log plots
    plt.xlabel(r'$\tilde{f}=\frac{f}{\hat{f}}$',fontsize=16)
    plt.ylabel(r'Probability Density $P(\tilde{f})$')
    plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
    plt.semilogy(forc_bin_centres, norm_forc_dens, '+', markersize=3)
    plt.savefig('probdens_forc_log.png')    
    plt.clf()
     
    plt.xlabel(r'$\tilde{p}=\frac{p}{\hat{p}}$',fontsize=16)
    plt.ylabel(r'Probability Density $P(\tilde{p})$')
    plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
    plt.semilogy(pres_bin_centres, norm_pres_dens, 'r+', markersize=3)
    plt.savefig('probdens_pres_log.png')    
    plt.clf()
     
    print ""
    print 'Mean radii: ', np.mean(final_radii)
    print 'Mean force: ', np.mean(final_force_data) 

    variance_force = np.var(final_force_data)
    standard_deviation_force = math.sqrt(variance_force)
    variance = np.var(final_radii)
    standard_deviation = math.sqrt(variance)

    print 'Variance force:', variance_force
    print 'Standard deviation force:', standard_deviation_force
    print 'Variance radii:', variance
    print 'Standard deviation radii:', standard_deviation
    print "Average Contact Number: ", np.mean(contact_number)
    print "5 Plots saved as pngs"




    #for i in range (0,len(array_pressure_distribution_g29)):
    #...     plt.plot(array_pressure_distribution_bi[i][1], array_pressure_distribution_bi[i][0], 'gD', markersize = 3)



    #with open(r'C:\Users\Anthony\Desktop\Physics Project\2D Polydisperse G-29 Graphs\G-29 Data\G-29 radii_distribution.dat') as file:
    #...     array_radii_distribution_g29 = [[float(digit) for digit in line.split()] for line in file]
