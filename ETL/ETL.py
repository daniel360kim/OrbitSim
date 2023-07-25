"""
Converts two line elements into the JSON format used by the simulation. 
https://celestrak.org/NORAD/documentation/tle-fmt.php 
"""

import json
import math

FILE_PATH = "celestrak.txt"

def convertName(line):
    columns = line.strip()
    name = columns[0:24]
    return {
        "name": name
    }

def convertLine1(line):
    columns = line.strip()

    satelliteNumber = columns[2:7]
    classification = columns[7]
    internationalDesignatorLaunchYear = columns[9:11]
    internationalDesignatorLaunchNumber = columns[11:14]
    internationalDesignatorPieceOfLaunch = columns[14:17]
    epochYear = columns[18:20]
    epoch = columns[20:32]
    checksum1 = columns[68]


    return {
        "satelliteNumber": satelliteNumber,
        "classification": classification,
        "internationalDesignatorLaunchYear": internationalDesignatorLaunchYear,
        "internationalDesignatorLaunchNumber": internationalDesignatorLaunchNumber,
        "internationalDesignatorPieceOfLaunch": internationalDesignatorPieceOfLaunch,
        "epochYear": epochYear,
        "epoch": epoch,
    }

def convertLine2(line):
    columns = line.strip()

    DEG_TO_RAD = math.pi / 180

    satelliteNumber = columns[2:7]
    inclination = float(columns[8:16]) * DEG_TO_RAD
    rightAscensionOfAscendingNode = float(columns[17:25]) * DEG_TO_RAD
    eccentricity = float("0." + columns[26:33])
    argumentOfPerigee = float(columns[34:42]) * DEG_TO_RAD
    meanMotion = float(columns[52:63])


    semiMajorAxis = (398600.4418 / (meanMotion * 2 * math.pi / 86400)**2)**(1/3)
    longitudeOfAscendingNode = rightAscensionOfAscendingNode - argumentOfPerigee

    return {
        "satelliteNumber": satelliteNumber,
        "inclination": inclination,
        "eccentricity": eccentricity,
        "argumentOfPerigee": argumentOfPerigee,
        "semiMajorAxis": semiMajorAxis,
        "longitudeOfAscendingNode": longitudeOfAscendingNode,

    }

def ETL(filename):
    with open(filename, "r") as file:
        lines = file.readlines()

    satellites = []
    for i in range(0, len(lines), 3):
        name = convertName(lines[i])
        line1 = convertLine1(lines[i+1])
        line2 = convertLine2(lines[i+2])
        satellite = {**name, **line1, **line2}
        satellites.append(satellite)
    
    with open("satellites.json", "w") as file:
        json.dump(satellites, file, indent=4)

        
        
        
        
    




ETL(FILE_PATH)

        
