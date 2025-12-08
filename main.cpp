#include <iostream>
#include <fstream>
#include <vector> 
#include <cmath>
#include "deps/sqlite3.h"

//For setting up sqlite3 if you get linker errors:
//For Linux:
//sudo apt update
//sudo apt install libsqlite3-dev
//g++ -o out main.cpp -lsqlite3

//The goat for doc of SQL tables:
//https://www.geeksforgeeks.org/cpp/sql-using-c-c-and-sqlite/

//Drone function
struct DroneHolder
{
    std::string type;
    std::string name;
    double mass;
    int maxSpeed;
    double B;
    int windSens;
    int powInp;
    double batVolt;
    double avgCur;
    double totPow;

    DroneHolder()
    {
        type = "";
        name = "";
        mass = 0.0;
        maxSpeed = 0;
        B = 0.0;
        windSens = 0;
        powInp = 0;
        batVolt = 0.0;
        avgCur = 0.0;
        totPow = 0.0;
    }

    //calculated using kinematics for a to plug in into total force of the drone (thrust is the acceleration from motors due to max speed)
    double getForce(double altitude)
    {
        double a = (pow(maxSpeed, 2))/(2*altitude);
        double force = mass * a + mass * 9.8;
        return force;
    }

    //obtained by multiplying force by distance from the center
    double getTorque(double force)
    {
        return force * B;
    }

    void printInfo()
    {
        std::cout << "Type: " << type << " Name: " << name << " Mass: " <<  mass <<std::endl;
        std::cout << "MaxSpeed: " << maxSpeed << " B: " << B << " Wind Sensitivity: " <<  windSens <<std::endl;
        std::cout << "Power Input: " << powInp << " Battery Voltage: " << batVolt << "  Average Current: " <<  avgCur <<std::endl;
        std::cout << "Total Power: " << totPow << std::endl;
    }

};



//SQL Parser for drone info, the functions returns string as error to determine if the code was able to run properly
std::string sqlParser(std::string mesg, DroneHolder& drone)
{
    //get the db
    sqlite3* db;
    int rc = sqlite3_open("drones.db", &db);
    if(rc != SQLITE_OK)
    {
        return "DIDNT OPEN PROPERLY";
    }

    //prepare the statement to get a drone column's info
    sqlite3_stmt* stmt;
    const char* sql = mesg.c_str();
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK)
    {
        return "STATEMENT ERROR";
    }
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        //Get all the info
        drone.type = (const char*)sqlite3_column_text(stmt,0);
        drone.name = (const char*)sqlite3_column_text(stmt,1);
        drone.mass = sqlite3_column_double(stmt, 2);
        drone.maxSpeed = sqlite3_column_int(stmt,3);
        drone.B = sqlite3_column_double(stmt, 4);
        drone.windSens = sqlite3_column_int(stmt,5);
        drone.powInp = sqlite3_column_int(stmt, 6);
        drone.batVolt = sqlite3_column_double(stmt,7);
        drone.avgCur = sqlite3_column_double(stmt,8);
        drone.totPow = sqlite3_column_double(stmt, 9);
        
        
    }
    if(rc != SQLITE_DONE)
    {
        return "DID NOT FINISH";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "";
}


int main()
{
    //ONLY 3 Options for Now
    std::vector<std::string> temp = {"DJIMavic", "WingtraOne", "RyzeTello"};


    //Parsing for User info
    std::cout << "Choose your Drone Model by ONLY typing in the number\n1-DJIMavic\n2-WingtraOne\n3-RyzeTello\nYourChoice: ";
    std::string choice;
    std::cin >> choice;
    int idx;
    try
    {
        idx = stoi(choice)-1;
        if(idx > 2 || idx < 0)
        {
            throw("Out of Range");
        }
    }
    catch(const char* msg)
    {
        std::cerr << msg << '\n';
        return 1;
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << "Didnt input a valid number possibly" << '\n';
        return 1;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Something Unexpected happen, look at code or logs" << '\n';
        return 1;
    }
    std::string stmtMesg = "SELECT * FROM Drones WHERE Model = '" + temp[idx] +"';";
    DroneHolder dr;
    try
    {
        std::string error = sqlParser(stmtMesg, dr);
        if(error != "")
        {
            throw(error);
        }
        std::cout << '\n';
        dr.printInfo();
        std::cout << '\n';
    }
    catch(std::string msg)
    {
        std::cerr << msg << '\n';
        return 1;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    //General simulation
    int windSensitivity;
    int windIdx;
    int altitude;
    try
    {
        std::vector<int> windSpeedBaeu = {1, 3, 11, 19, 28, 38, 49, 61, 74, 88, 102, 117, 118}; //wind speeds based off the Beaucraut scale from https://en.wikipedia.org/wiki/Beaufort_scale
        std::cout << "Calm:0(1km/h)\nLight Air:1(3km/h)\nLight breeze:2(11km/h)\nGentle breeze:3(19km/h)\nModerate breeze:4(28km/h)\nFresh breeze:5(38km/h)\nStrong Breeze:6(49km/h)\nModerate gale:7(61km/h)\nGale:8(74km/h)\nStrong gale:9(88km/h)\nStorm:10(102km/h)\nViolent storm:11(117km/h)\nHurricane-force:12(118km/h)\nChooseChoice: ";
        
        std::cin >> windIdx;
        if(windIdx > 12)
        {
            throw("OutofRange");
        }
        windSensitivity = windSpeedBaeu[windIdx];
        std::cout << '\n';
        std::cout << "Altitude(int) in m: \n";
        
        std::cin >> altitude;
        std::cout << '\n';
    }
    catch(const std::out_of_range& e)
    {
        std::cout << "Out of range: " << e.what() << '\n';
        return 1;
    }
    catch(const char* e)
    {
        std::cout << "Out of range"<< '\n';
        return 1; 
    }
    catch(const std::exception& e)
    {
        std::cout << "Something when horribly wrong: " << e.what() << '\n';
        return 1;
    }

    //calculate drone force and torque for simulation
    double currDroneForce = dr.getForce(altitude);
    double currDroneTorque = dr.getTorque(currDroneForce);

    //run this loop until the power runs out
    double powerLimit = dr.powInp;
    double baseUse = (currDroneTorque) + dr.totPow;
    double batOut = baseUse;
    double totalUsage;
    int timeOfRun;
    while(powerLimit> 0)
    {
        if(windSensitivity > dr.windSens)
        {
            batOut+=0.1; //incrementing damage to drone
        }
        timeOfRun++;
        totalUsage += batOut;
        powerLimit-=batOut;
    }
    
    //print results
    std::cout << "Total Time (S): " << timeOfRun << std::endl;
    std::cout << "Total Energy Used (J): " << dr.totPow * timeOfRun << std::endl;
    std::cout << "Total Usage over Time (V): " << totalUsage << std::endl;
    std::cout<< "Drone Info: " << std::endl;
    dr.printInfo();
    //end of simulation so reset if needed


    
    
    return 0;
}