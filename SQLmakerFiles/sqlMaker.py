import sqlite3
import csv
import os

#temp location for now, need to figure out how to integrate this later or just make a temp for now
#Reader for Doc
listOfDroneInfo = []
with open('SQLmakerFiles/OfficialDroneDoc.csv', newline='') as csvFile:
    spamreader = csv.reader(csvFile, delimiter=' ', quotechar= '|')
    for row in spamreader:
        listOfDroneInfo.append(row)
        #print(''.join(row))
#SQL maker, for Linux
with sqlite3.connect('drones.db') as conn:
    Cursor = conn.cursor()
    # title = f"""CREATE TABLE Drones({listOfDroneInfo[0][0][0:-1]}, {listOfDroneInfo[0][1][0:-1]}, {listOfDroneInfo[0][2][0:-1]}, {listOfDroneInfo[0][3][0:-1]}, {listOfDroneInfo[0][4][0:-1]}, {listOfDroneInfo[0][5][0:-1]}, {listOfDroneInfo[0][6][0:-1]}, {listOfDroneInfo[0][7][0:-1]}, {listOfDroneInfo[0][8][0:-1]}, {listOfDroneInfo[0][9]})"""
    # Cursor.execute(title)
    # print("Titles")
    # print(title)

    #Check if added
    # checkDBCreate = Cursor.execute("SELECT name from sqlite_master")
    # print(checkDBCreate.fetchall())
    # print(listOfDroneInfo[0][2][0:-1])
    #Only 3 examples
    for i in range(1, 4):
        task = f"""INSERT INTO Drones VALUES ("{listOfDroneInfo[i][0][0:-1]}", "{listOfDroneInfo[i][1][0:-1]}", {listOfDroneInfo[i][2][0:-1]}, {listOfDroneInfo[i][3][0:-1]}, {listOfDroneInfo[i][4][0:-1]}, {listOfDroneInfo[i][5][0:-1]}, {listOfDroneInfo[i][6][0:-1]}, {listOfDroneInfo[i][7][0:-1]}, {listOfDroneInfo[i][8][0:-1]}, {listOfDroneInfo[i][9][0:-1]})"""
        print("Tasks")
        print(task)
        Cursor.execute(task)
        conn.commit()
        #Test to see if added
        res = Cursor.execute("SELECT TypeofDrone FROM Drones")
        print()
        print(res.fetchall())
    Cursor.close()

