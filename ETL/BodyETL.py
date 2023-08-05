import pandas as pd
import numpy as np
import json

FILE_PATH = "Input/exoplanets.csv"


### Solar System Data (not included in the exoplanet data set) ###
SOLAR_SYSTEM_NAMES = []
SOLAR_SYSTEM_NAMES.append("Mercury")
SOLAR_SYSTEM_NAMES.append("Venus")
SOLAR_SYSTEM_NAMES.append("Earth")
SOLAR_SYSTEM_NAMES.append("Mars")
SOLAR_SYSTEM_NAMES.append("Jupiter")
SOLAR_SYSTEM_NAMES.append("Saturn")
SOLAR_SYSTEM_NAMES.append("Uranus")
SOLAR_SYSTEM_NAMES.append("Neptune")
SOLAR_SYSTEM_NAMES.append("Pluto (dwarf planet)")

SOLAR_SYSTEM_DISCOVERY_YEARS = []
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Mercury
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Venus
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Earth
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Mars
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Jupiter
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Saturn
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Uranus
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Neptune
SOLAR_SYSTEM_DISCOVERY_YEARS.append(0) # Pluto (dwarf planet)


SOLAR_SYSTEM_MASSES = []
SOLAR_SYSTEM_MASSES.append(3.301 * 10**23) # Mercury
SOLAR_SYSTEM_MASSES.append(4.867 * 10**24) # Venus
SOLAR_SYSTEM_MASSES.append(5.972 * 10**24) # Earth
SOLAR_SYSTEM_MASSES.append(6.39 * 10**23) # Mars
SOLAR_SYSTEM_MASSES.append(1.898 * 10**27) # Jupiter
SOLAR_SYSTEM_MASSES.append(5.683 * 10**26) # Saturn
SOLAR_SYSTEM_MASSES.append(8.681 * 10**25) # Uranus
SOLAR_SYSTEM_MASSES.append(1.024 * 10**26) # Neptune
SOLAR_SYSTEM_MASSES.append(1.309 * 10**22) # Pluto (dwarf planet)

SOLAR_SYSTEM_RADII = []
SOLAR_SYSTEM_RADII.append(2.4397 * 10**6) # Mercury
SOLAR_SYSTEM_RADII.append(6.0518 * 10**6) # Venus
SOLAR_SYSTEM_RADII.append(6.371 * 10**6) # Earth
SOLAR_SYSTEM_RADII.append(3.3895 * 10**6) # Mars
SOLAR_SYSTEM_RADII.append(6.9911 * 10**7) # Jupiter
SOLAR_SYSTEM_RADII.append(5.8232 * 10**7) # Saturn
SOLAR_SYSTEM_RADII.append(2.5362 * 10**7) # Uranus
SOLAR_SYSTEM_RADII.append(2.4622 * 10**7) # Neptune
SOLAR_SYSTEM_RADII.append(1.1883 * 10**6) # Pluto (dwarf planet)

SOLAR_SYTEM_TYPES = []
SOLAR_SYTEM_TYPES.append("Terrestrial") # Mercury
SOLAR_SYTEM_TYPES.append("Terrestrial") # Venus
SOLAR_SYTEM_TYPES.append("Terrestrial") # Earth
SOLAR_SYTEM_TYPES.append("Terrestrial") # Mars
SOLAR_SYTEM_TYPES.append("Gas Giant") # Jupiter
SOLAR_SYTEM_TYPES.append("Gas Giant") # Saturn
SOLAR_SYTEM_TYPES.append("Gas Giant") # Uranus
SOLAR_SYTEM_TYPES.append("Gas Giant") # Neptune
SOLAR_SYTEM_TYPES.append("Terrestrial") # Pluto (dwarf planet)




def convert_to_kg(planet_mass_sin_i):
    jupiter_mass = 1.898 * 10**27
    return planet_mass_sin_i * jupiter_mass


def convert_to_meters(planet_radius):
    jupiter_radius = 6.9911 * 10**7
    return planet_radius * jupiter_radius

def is_column_numeric(df, column_name):
    try:
        pd.to_numeric(df[column_name])
        return True
    except ValueError:
        return False
    
    

def ETL(filename):
    df = pd.read_csv(filename)
    headers = list(df.columns)
    print("Successfully read %d rows and %d columns" % (len(df), len(headers)))

    json_file = []
    # iterate through each row

    # Add the solar system data to the json file
    for i in range(len(SOLAR_SYSTEM_NAMES)):
        json_element = {
            "planet_name": SOLAR_SYSTEM_NAMES[i],
            "discovery_year": SOLAR_SYSTEM_DISCOVERY_YEARS[i],
            "id": i + len(df),
            "mass": SOLAR_SYSTEM_MASSES[i],
            "radius": SOLAR_SYSTEM_RADII[i],
            "planet_type": SOLAR_SYTEM_TYPES[i],
        }
        json_file.append(json_element)


    for i in range(len(df)):
        add_to_json = False
        row = df.iloc[i]

        planet_name = row["# name"]
        discovery_year = row["discovered"]
        discovery_year_is_valid = False
        if is_column_numeric(df, "discovered") and not pd.isna(discovery_year):
            discovery_year = int(discovery_year)
            discovery_year_is_valid = True
        
        id = i
        mass = row["mass"]
        # Chech to see if the planet mass is empty and if the value is a number
        mass_is_valid = False
        if is_column_numeric(df, "mass") and not pd.isna(mass):
            mass = convert_to_kg(mass)
            mass_is_valid = True
        
        radius_is_valid = False
        planet_radius = row["radius"]
        if is_column_numeric(df, "radius") and not pd.isna(planet_radius):
            planet_radius = convert_to_meters(planet_radius)
            radius_is_valid = True
        
        EARTH_MASS = 5.972 * 10**24
        EARTH_RADIUS = 6.371 * 10**6
        
        planet_type = "Terrestrial"
        if planet_radius > 4 * EARTH_RADIUS and mass > 10 * EARTH_MASS:
            planet_type = "Gas Giant"

        if discovery_year_is_valid and mass_is_valid and radius_is_valid:
            json_element = {
                "planet_name": planet_name,
                "discovery_year": discovery_year,
                "id": id,
                "mass": mass,
                "radius": planet_radius,
                "planet_type": planet_type
            }
            json_file.append(json_element)


    

    

    with open("Output/exoplanets.json", "w") as file:
        json.dump(json_file, file, indent=4)


        

            



 


            
    





ETL(FILE_PATH)

