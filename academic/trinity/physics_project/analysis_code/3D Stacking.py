########################################################
# Python Script for calculating overlaps in 3D packing #
########################################################
 
#Import Libraries
import numpy as np
import scipy.integrate as sp
import matplotlib.pyplot as plt
import sys
 
###Parameters###
 
#PI
pi=3.141592653589793238462
 
#Initialise rattler count
rattlers = 0
 
#Constant for harmonic repulsion ### Not necessary as force get non-dimensionalised
h_const = 100000
 
#Bin size for histogram
bin_size = 0.05
 
#Initialise arrays 
norm_forc_dens = []
norm_pres_dens = []
contact_number = []
 
###Functions###
 
#Function for calulating overlaps in periodic packing
def overlap (x1, y1, z1, r1, x2, y2, z2, r2):
     
    distance = np.sqrt( (x1 - x2)**2 + (y1 - y2)**2 + (z1 - z2)**2)
     
    #Recalculate overlaps at boundary
    if (distance > r1 + r2):
 
        newdistance = distance
        if abs(x1 - x2) > (r1 + r2):
            newdistance = np.sqrt((1 - abs(x1 - x2))**2 + (y1 - y2)**2 + (z1 - z2)**2)
 
        if abs(y2 - y1) > (r1 + r2):
            newdistance = np.sqrt((x1 - x2)**2 + (1 - abs(y1 - y2))**2 + (z1 - z2)**2)
            
        if abs(z2 - z1) > (r1 + r2):
            newdistance = np.sqrt((x1 - x2)**2 + (y1 - y2)**2 + (1 - abs(z1 - z2))**2)
 
        if abs(y2 - y1) > (r1 + r2) and abs(x1 - x2) > (r1 + r2):
            newdistance = np.sqrt((1 - abs(x1 - x2))**2 + (1 - abs(y1 - y2))**2 + (z1 - z2)**2)
            
        if abs(y2 - y1) > (r1 + r2) and abs(z1 - z2) > (r1 + r2):
            newdistance = np.sqrt((x1 - x2)**2 + (1 - abs(z1 - z2))**2 + (1 - abs(y1 - y2))**2)
            
        if abs(z2 - z1) > (r1 + r2) and abs(x1 - x2) > (r1 + r2):
            newdistance = np.sqrt((1 - abs(x1 - x2))**2 + (y1 - y2)**2 + (1 - abs(z1 - z2))**2)
            
        if abs(y2 - y1) > (r1 + r2) and abs(x1 - x2) > (r1 + r2) and abs(z2 - z1) > (r1 + r2):
            newdistance = np.sqrt((1 - abs(x1 - x2))**2 + (1 - abs(y1 - y2))**2 + (1 - abs(z1 - z2))**2)
 
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
 
#Loop through files 
for directory in range(1,500):   
    contact_file= r'C:\Users\Anthony\Desktop\Physics Project\3D Monodisperse\\'  + str(directory) + '\\contacts_new.dat'
    coordinate_file = r'C:\Users\Anthony\Desktop\Physics Project\3D Monodisperse\\'  + str(directory) + '\\final_new.dat'
     
    
    #Import Data
    with open(contact_file) as file:
        arraycon = [[int(digit) for digit in line.split()] for line in file]
         
    with open(coordinate_file) as file:
        arraypos = [[float(digit) for digit in line.split()] for line in file]
   
   # #Prints progress
    percentage = directory * 0.2
    sys.stdout.write("\r%d%%" % percentage)
    sys.stdout.flush()
         
 
    #Initialise array specific to each data set
    force_data = []
    pressure_data = []
    radii = []
 
    total_balls = len(arraypos)
 
    #Loop over every sphere
    for i in range (0, total_balls - 1):
 
        ball_i = i
        number_of_contacts = arraycon[ball_i][0]
        force_on_ball_i = []
 
        x1 = arraypos[ball_i][1] ##Prepared for files with no ballnumber column
        y1 = arraypos[ball_i][2]
        z1 = arraypos[ball_i][3]
        r1 = arraypos[ball_i][4]
 
        radii.append(r1)
 
        if (number_of_contacts != 0):
 
            contact_number.append(number_of_contacts)
 
            #Loop over overlapping spheres
            for j in range (1, number_of_contacts):
 
                ball_j = arraycon[ball_i][j] - 1
 
                x2 = arraypos[ball_j][1]
                y2 = arraypos[ball_j][2]
                z2 = arraypos[ball_j][3]
                r2 = arraypos[ball_j][4]
 
                #Overlap calculation
                delta = overlap (x1, y1, z1, r1, x2, y2, z2, r2)
 
                #Force
                force = h_const * delta
                force_data.append(force)
               
                #Pressure
            force_on_ball_i.append(force)
              
             
            pressure = (sum(force_on_ball_i)) / ( 4 * pi * ((r1)**2) )
            pressure_data.append(pressure)
             
        else:
            #count rattlers
            rattlers += 1
 
 
    ###Normalising Data###
 
    #Non-dimensionalise force and pressure
    force_data = force_data / np.mean(force_data)
    pressure_data = pressure_data / np.mean(pressure_data)
 
 
    #Generate histograms  ###  Fixed the range because I couldn't add arrays of different lengths
    forc_x_axis = np.arange(0, 5, bin_size)
    pres_x_axis = np.arange(0, 5, bin_size) 
 
    forc_dens , forc_bin_edges = np.histogram(force_data,    bins = forc_x_axis)
    pres_dens , pres_bin_edges = np.histogram(pressure_data, bins = pres_x_axis)
 
 
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
 
 
    #Print Mean radii
    print 'Mean radii: ', np.mean(radii)
 
 
 
###Plot histograms###
forc_bin_centres = (forc_bin_edges[:-1] + forc_bin_edges[1:])/2
pres_bin_centres = (pres_bin_edges[:-1] + pres_bin_edges[1:])/2
 
norm_forc_dens = norm_forc_dens  / sp.simps(norm_forc_dens, dx = bin_size)
norm_pres_dens = norm_pres_dens  / sp.simps(norm_pres_dens, dx = bin_size)
 

write_to_file(forc_bin_centres, norm_forc_dens, 'forc_3ddens.dat')
write_to_file(pres_bin_centres, norm_pres_dens, 'pres_3ddens.dat')
 
plt.xlabel(r'$\tilde{f}=\frac{f}{\hat{f}}$',fontsize=16)
plt.ylabel(r'Probability Density $P(\tilde{f})$')
plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
plt.plot(forc_bin_centres, norm_forc_dens, 'D', markersize=3)
plt.savefig('probdens_forc.png')    
plt.clf()
plt.show()
 
plt.xlabel(r'$\tilde{p}=\frac{p}{\hat{p}}$',fontsize=16)
plt.ylabel(r'Probability Density $P(\tilde{p})$')
plt.gcf().subplots_adjust(bottom=0.15) #Adjusts figure to make room for label
plt.plot(pres_bin_centres, norm_pres_dens, 'rD', markersize=3)
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
print "Average Contact Number: ", np.mean(contact_number)
print "4 Plots saved as pngs"
