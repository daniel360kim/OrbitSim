import pandas as pd
import plotly.graph_objects as go

# Read the CSV file
df = pd.read_csv('../Simulator/out/prop_data46.csv')

# Extract data from the DataFrame
x = df['Position X'] - df['Position X'].mean()  # Subtract mean to center around (0, 0, 0)
y = df['Position Y'] - df['Position Y'].mean()  # Subtract mean to center around (0, 0, 0)
z = df['Position Z'] - df['Position Z'].mean()  # Subtract mean to center around (0, 0, 0)

# Create a 3D plot
fig = go.Figure()

# Plot the Earth
earth_radius = 6371  # Radius of the Earth in kilometers
u = df['Time']
v = df['True Anomaly']
earth_x = earth_radius * pd.Series([0] * len(u))
earth_y = earth_radius * pd.Series([0] * len(u))
earth_z = earth_radius * pd.Series([0] * len(u))
fig.add_trace(go.Scatter3d(x=earth_x, y=earth_y, z=earth_z, mode='markers', marker=dict(color='blue', size=2), name='Earth'))

# Plot the spacecraft's orbit
fig.add_trace(go.Scatter3d(x=x, y=y, z=z, mode='lines', line=dict(color='red'), name='Spacecraft Orbit'))

# Set layout
fig.update_layout(scene=dict(aspectmode='data', xaxis_title='X', yaxis_title='Y', zaxis_title='Z'), title='Spacecraft Orbit with Earth')

# Show the plot
fig.show()
