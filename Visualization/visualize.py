import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import numpy as np

# Read the CSV file
df = pd.read_csv('../Simulator/out/prop_data46.csv')

# Extract data from the DataFrame
time = df['Time']
true_anomaly = df['True Anomaly']
x = df['Position X']
y = df['Position Y']
z = df['Position Z']

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Create a sphere for the Earth
radius = 6371
u = np.linspace(0, 2 * np.pi, 100)
v = np.linspace(0, np.pi, 50)
x_earth = radius * np.outer(np.cos(u), np.sin(v))
y_earth = radius * np.outer(np.sin(u), np.sin(v))
z_earth = radius * np.outer(np.ones(np.size(u)), np.cos(v))
ax.plot_surface(x_earth, y_earth, z_earth, cmap=cm.Blues, alpha=0.8)

# Plot the spacecraft's orbit
ax.plot(x, y, z)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Spacecraft Orbit')

plt.show()
