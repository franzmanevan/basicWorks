/* Copyright [2020] <Evan Franzman>
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   franzmel_hw3.cpp
 * Author: evanfranzman
 *
 * Created on February 13, 2020, 10:34 PM
 */

#include <cstdlib>
#include <string>
#include "franzmel_hw3.h"

Map::Map() {}
Map::Map(const Map& orig) {
    this->ppid = orig.ppid;
    this->cmd = orig.cmd;
}
Map::~Map() {}
std::string Map::getcmdValue(std::string key) {
    return this->cmd[key];
}
std::string Map::getppidValue(std::string key) {
    return this->ppid[key];
}
void Map::addcmd(std::string key, std::string value) {
    cmd[key] = value;
}
void Map::addppid(std::string key, std::string value) {
    ppid[key] = value;
}
void Map::parseLine(std::string& line, std::string& pid, 
        std::string& ppid, std::string& cmd) {
    // Declaring an input string stream to simplify the process of reading in
    // the values of 'line' that we care about (PID, PPID, CMD).
    std::istringstream str(line);
    // Declaring dummy strings to help us ignore the values in 'line' that are
    // unimportant.
    std::string dummy1, dummy2, dummy3, dummy4, dummy5, dummy6;
    // Reads in the values. The ones that are unimportant go into the dummy
    // variables which will be removed off of the stack after the method has 
    // finished. The values we do care about will be stored in the reference 
    // variables with a greater scope.
    str >> dummy1 >> pid >> ppid >> dummy3 >> dummy4 >> dummy5 >> dummy6;
    // One more temporary variable to help read in the CMD from each line.
    std::string temp;
    // Because some of the commands have spaces in them, in order to ensure that
    // we read the whole command into CMD, we can use getline to read the rest
    // of the string into cmd.
    std::getline(str, cmd);
}
void Map::load(std::ifstream& in) {    
    // Declaring std::string variable to assign line values being read in from
    // the specified file. 
    std::string line;
    // Loop that will read in every line of the file, line by line and store
    // PID, PPID, and CMD values into the two instance variables of the object.
    while (std::getline(in, line)) {
        // Strings being declared to later be stored into the two instance
        // variable unordered_maps as <PID, CMD> and <PID, PPID> respectively.
        std::string pid, ppid, cmd;
        // Private helper method call that will take the strings as reference 
        // variables and store the values PID, PPID, and CMD into the variables
        // respectively.
        parseLine(line, pid, ppid, cmd);
        // Adds a new PID and CMD key/value pair to the instance variable
        // unordered_map of the object.
        this->addcmd(pid, cmd);
        // Adds a new PID and PPID key/value pair to the instance variable
        // unordered_map of the object.
        this->addppid(pid, ppid);
    }
}
void Map::recHelp(const std::string& pid, std::string& procTree) {
    // Specify the base case of the recursive method. The base case is where
    // the first process is. This process is indicated by a pid of 1 and a 
    // ppid of 0. The rest of the scenarios will be in the else block.
    if (this->getppidValue(pid) == "0" && pid == "1") {
        // Adds the pid, ppid, and cmd separated by a tab as specified in the
        // homework document.
        procTree += pid + "\t" + this->getppidValue(pid) + "\t" + 
                this->getcmdValue(pid) + "\n";
    } else {
        // Recursive call is placed before adding current information to the 
        // tree so the order is descending starting from the pid 1.
        recHelp(this->getppidValue(pid), procTree);
        procTree += pid + "\t" + this->getppidValue(pid) + "\t" + 
                this->getcmdValue(pid) + "\n";
    }
}
void Map::printTree(std::ostream& out, const std::string& pid) {
    // String that will store the formatted process tree.
    std::string procTree;
    // Recursive helper method that initializes procTree with the corresponding
    // process tree to pid.
    recHelp(pid, procTree);
    // Displays the formatted process tree as specified per the homework doc.
    out << "Process tree for PID: " << pid << std::endl
        << "PID\tPPID\tCMD" << std::endl
        << procTree;
}
int main(int argc, char** argv) {
    // File input stream to the file specified by command line argument.
    std::ifstream in(argv[1]);  // ***Change after testing***
    // PID that we are identifying the Process Tree for; Specified via 
    // command line argument.
    std::string pid = argv[2];  // ***Change after testing***
    // Map object containing two unordered_map instance variables with both key
    // and value pairs being of type std::string. Used to identify and store the
    // PID, PPID, and CMD for each line read in from the specified file.
    Map tree;
    // Method call on the Map object to load in and store the PID, PPID, and CMD
    // from each line of the specified file.
    tree.load(in);
    // Method call on the Map object to print out the Process Tree to the
    // corresponding, specified PID.
    tree.printTree(std::cout, pid);
    return 0;
}
