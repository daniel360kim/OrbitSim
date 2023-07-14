path = '../Simulator/out/prop_data54.csv'

import csv
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.integrate import odeint

# Setting up Spherical Earth to Plot
N = 50
phi = np.linspace(0, 2 * np.pi, N)
theta = np.linspace(0, np.pi, N)
theta, phi = np.meshgrid(theta, phi)

r_Earth = 6378.14  # Average radius of Earth [km]
X_Earth = r_Earth * np.cos(phi) * np.sin(theta)
Y_Earth = r_Earth * np.sin(phi) * np.sin(theta)
Z_Earth = r_Earth * np.cos(theta)

# Plotting Earth
fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_surface(X_Earth, Y_Earth, Z_Earth, color='blue', alpha=0.7)

# Reading in Data
time = []
true_anomaly = []
position = []
velocity = []



with open(path, 'r') as file:
    print('Reading in Data...')
    csv_reader = csv.reader(file)
    next(csv_reader)  # Skip header row
    
    ## Go through each row in the csv file if NaN then skip
    for row in csv_reader:
        time.append(float(row[0]))
        true_anomaly.append(float(row[1]))
        position.append([float(row[2]), float(row[3]), float(row[4])])
        velocity.append([float(row[5]), float(row[6]), float(row[7])])
        
        

X_Position = [x[0] for x in position]
Y_Position = [x[1] for x in position]
Z_Position = [x[2] for x in position]

# Plotting Orbit
ax.plot3D(X_Position, Y_Position, Z_Position, color='red', alpha=0.7)




ax.view_init(30, 145)  # Changing viewing angle (adjust as needed)
plt.title('Two-Body Orbit')
ax.set_xlabel('X [km]')
ax.set_ylabel('Y [km]')
ax.set_zlabel('Z [km]')

plt.show()
