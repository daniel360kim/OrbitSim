

path = '../Simulator/out/prop_data4.csv'

import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Read the CSV file
df = pd.read_csv(path)

# Extract data from the DataFrame
time = df['Time']
true_anomaly = df['True Anomaly']
x = df['X']
y = df['Y']
z = df['Z']

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot(x, y, z)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Spacecraft Orbit')

plt.show()
