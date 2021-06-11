/* Copyright [2020] <Evan Franzman>
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   franzmel_hw3.h
 * Author: evanfranzman
 *
 * Created on February 13, 2020, 10:35 PM
 */

#ifndef FRANZMEL_HW3_H
#define FRANZMEL_HW3_H
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>

class Map {
private:
    /**
     * Instance variables that are meant to ease the storage and retrieval of 
     * pid, ppid, and cmd values by keeping them in key/value pairs, with the 
     * pid as a consistent key.
     */
    std::unordered_map<std::string, std::string> ppid, cmd;
    /**
     * Private helper method that parses data on line and reads in important values
     * into pid, ppid, and cmd.
     * @param line String reference that will be altered in order to extract data.
     * @param pid String reference meant to store the pid value from a line .
     * @param ppid String reference meant to store the ppid value from a line.
     * @param cmd String reference meant to store the cmd value from a line.
     */
    void parseLine(std::string& line, std::string& pid,
    std::string& ppid, std::string& cmd);
    /**
     * Private, recursive helper method designed to print out the process tree 
     * of a specified pid starting from root and ending at the pid.
     * @param pid Specified pid that will have the process tree printed out to.
     * @param procTree String reference that stores the formatted pid, ppid and
     * cmd values in descending order starting from pid 1.
     */
    void recHelp(const std::string& pid, std::string& procTree);
    
public:
    /**
     * Standard zero argument constructor for the Map class.
     */
    Map();
    /**
     * Standard copy constructor for the map class.
     * @param orig Map object that a copy is being made for.
     */
    Map(const Map& orig);
    /**
     * Standard destructor for the Map class.
     */
    ~Map();
    /**
     * Accessor method for the ppid instance variable.
     * @param key Specified pid key that is used to find corresponding ppid value.
     * @return The corresponding ppid value to the given pid key.
     */
    std::string getppidValue(std::string key);
    /**
     * Accessor method for the cmd instance variable.
     * @param key Specified pid key that is used to find corresponding cmd value.
     * @return The corresponding cmd value to the given pid key
     */
    std::string getcmdValue(std::string key);
    /**
     * Mutator method to add string key/value pair to the cmd instance variable.
     * @param key Specified pid key that will be added to the cmd instance variable.
     * @param value Specified cmd value that will be added to the cmd instance 
     * variable alongside 'key'.
     */
    void addcmd(std::string key, std::string value);
    /**
     * Mutator method to add string key/value pair to the ppid instance variable.
     * @param key Specified pid key that will be added to the ppid instance variable.
     * @param value Specified ppid value that will be added to the ppid instance
     * variable alongside 'key'.
     */
    void addppid(std::string key, std::string value);
    /**
     * Mutator method that takes pid, ppid, and cmd values from a file and stores
     * them into an objects intance variable unordered_maps.
     * @param in File stream reference that is used to read in data line by line
     * from the file.
     */
    void load(std::ifstream& in);
    /**
     * Accessor method that prints out the process tree for a given pid.
     * @param out Output stream reference that is used to output data to the console.
     * @param pid Specified pid value that will be used to determine the process tree.
     */
    void printTree(std::ostream& out, const std::string& pid);
};

#endif /* FRANZMEL_HW3_H */

