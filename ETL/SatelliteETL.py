"""
Converts two line elements into the JSON format used by the simulation. 
https://celestrak.org/NORAD/documentation/tle-fmt.php 
"""

import json
import math
from datetime import datetime, timedelta

FILE_PATH = "Input/celestrak.txt"

def convertName(line):
    columns = line.strip()
    name = columns[0:24]
    return {
        "name": name
    }

def convertYear(year):
    if int(year) < 57: # Sputnik 1 was launched in 1957
        return "20" + year
    else:
        return "19" + year

def isLeapYear(year):
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)
    
def convertEpoch(epoch, year):
    # epoch is the day and the fraction of the day
    # epochYear is the last two digits of the year

    day_of_year = int(epoch)
    fraction_of_day = epoch - day_of_year

    days_in_month = [31,28,31,30,31,30,31,31,30,31,30,31]

    if isLeapYear(year):
        days_in_month[1] = 29

    month = 0
    day = 0
    for i in range(12):
        if day_of_year - days_in_month[i] < 0:
            month = i
            day = day_of_year
            break
        else:
            day_of_year -= days_in_month[i]
    

    


    hour = int(fraction_of_day * 24)
    minute = int((fraction_of_day * 24 - hour) * 60)

    return month + 1, day, hour, minute
    

def convertLine1(line):
    columns = line.strip()

    satelliteNumber = columns[2:7]
    classification = columns[7]
    internationalDesignatorLaunchYear = columns[9:11]
    internationalDesignatorLaunchNumber = columns[11:14]
    internationalDesignatorPieceOfLaunch = columns[14:17]
    epochYear = columns[18:20]
    year = convertYear(epochYear)
    epoch = columns[20:32]
    datetime= convertEpoch(float(epoch), int(year))
    checksum1 = columns[68]


    return {
        "satelliteNumber": satelliteNumber,
        "classification": classification,
        "internationalDesignatorLaunchYear": internationalDesignatorLaunchYear,
        "internationalDesignatorLaunchNumber": internationalDesignatorLaunchNumber,
        "internationalDesignatorPieceOfLaunch": internationalDesignatorPieceOfLaunch,
        "epochYear": epochYear,
        "epoch": epoch,
        "year": int(year),
        "month": datetime[0],
        "day": datetime[1],
        "hour": datetime[2],
        "minute": datetime[3],
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
    
    with open("Output/satellites.json", "w") as file:
        json.dump(satellites, file, indent=4)

        
        
        
        
    




ETL(FILE_PATH)

        
