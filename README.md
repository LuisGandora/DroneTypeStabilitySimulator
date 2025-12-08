# DroneTypeStabilitySimulator
Simulates drone stability using C++
12/7 Drone Logic Finished
Author-Luis Andre Matudio Blanco
Deps-SQLite3 Doc pulled from website(https://sqlite.org/docs.html)

The goal of this project is to make a visual drone simluator to showcase stability of various drones from a db to scour via multiple SQL API's for practice.

The project documentation is split into the following chapters:
    1. Python CSV->SQL DB file maker
    2. C++ Drone Simulation
        -> Drone Struct
        ->SQL Parser
        ->Main simulation

-Python CSV->SQL DB file maker
    Libraries:
        sqlite3, csv, os
    The CSV stores info about drones into these categories:
    Ex.
        | TypeofDrone | Model | Mass | VfMax | B | Wind | PowerInput | BatteryVoltage | AverageCurrent | TotalPower |
        | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
        | MultiRotorDrones | DJIMavic | 0.895 | 8 | 0.448 | 12 | 240 | 15 | 4.3 | 64.5 |
    Then using a combination of csv reader and os, info from the csv is stored in a python list of string lists
    '''python
    listOfDroneInfo = []
    with open('SQLmakerFiles/OfficialDroneDoc.csv', newline='') as csvFile:
        spamreader = csv.reader(csvFile, delimiter=' ', quotechar= '|')
        for row in spamreader:
            listOfDroneInfo.append(row)
    '''
    After the contents have been extracted, the process of inserting each element of drone info into a drone.db (drone database) happens. Which can be used for the drone simulation in C++

-C++ Drone Simulation
    Libraries:
        iostream, fstream, vector, cmath, sqlite.h
    Setup stuff:
        1. The files for sqlite3's api for c++ should be given in deps/ of this project. However if it is not there go to this website (https://sqlite.org/download.html) download the amalgamation file and extract these files
            ->shell.c
            ->sqlite.c
            ->sqlite3.h
            ->sqlite3ext.h
        2.Set up the dependencies (Linux build, look up windows equivelent if needed)
            ->sudo apt update
            ->sudo apt install libsqlite3-dev
            ->g++ -o out main.cpp -lsqlite3
    
