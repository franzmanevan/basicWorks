/* 
 * File:   franzmel_hw4.cpp
 * Author: evanfranzman
 *
 * Created on February 25, 2020, 2:37 PM
 */

#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>

void myExec(std::vector<std::string> argList) {
    // list of pointers to args
    std::vector<char*> args;
    for (auto& s : argList) {
        // address of 1st character
        args.push_back(&s[0]);
    }
    // nullptr is very important
    args.push_back(nullptr);
    // Make execvp system call to run desired process
    execvp(args[0], &args[0]);
    // In case execvp ever fails, we throw a runtime execption
    throw std::runtime_error("Call to execvp failed!");
}

int forkNexec(const std::vector<std::string> strVec) {
    // Fork and save the pid of the child process
    int childPid = fork();
    // Call the myExec helper method in the child
    if (childPid == 0) {
        // We are in the child process
        myExec(strVec);
    }
    // Control drops here only in the parent process!
    return childPid;
}
/**
 * 
 * @param file
 */
void serial(const std::string file) {    
    // String used to get an entire line from the file.    
    std::string line;
    // Stream that extracts from the file.
    std::ifstream in(file);
    // Loop that will run until there are no more lines in the file. It will
    // execute one line in the file at a time.
    while (std::getline(in, line)) {
        // Vector that will contain the command and the following arguments.
        std::vector<std::string> vec;
        // Skips over any lines that are comments or are empty.
        if ((line[0] == '#') || (line == "")) {
            continue;
        }
        // Stream that will extract arguments from a line and deposit them into
        // arg.
        std::stringstream ss(line);
        // String that will store the cmd, and any one argument to be put into
        // the vector.
        std::string arg;
        // Output that aligns with the output in the homework file.
        std::cout << "Running:";
        // While there are values on the line, it will put them into arg, one
        // at a time and push them into the vector.
        while (ss >> arg) {
            vec.push_back(arg);
            std::cout << " " << arg;
        }
        // Output that aligns with the output in the homework file.
        std::cout << std::endl;
        // Stores the childPid of the system call made in forkNexec.
        int childPid = forkNexec(vec);
        // Exit code to be modified and printed later
        int exitCode = 0;
        // Waits for the process to be done before continuing.
        waitpid(childPid, &exitCode, 0);
        // Output that aligns with the output in the homework file.
        std::cout << "Exit code: " << exitCode << std::endl;
    }
}
/**
 * Takes a file name and executes the contents inside of it in parallel.
 * @param file Filename that contains executable code to be run.
 */
void parallel(const std::string file) {
    // String used to get an entire line from the file.
    std::string line;
    // Stream that extracts from the file.
    std::ifstream in(file);
    // Vector of ints that stores the childPids for each process that is run.
    std::vector<int> children;
    // Exit code to be modified and printed later
    int exitCode = 0;
    // Loop that will run until there are no more lines in the file. It will
    // execute one line in the file at a time.
    while (std::getline(in, line)) {
        // Skips over any lines that are comments or are empty.
        if ((line[0] == '#') || (line == "")) {
            continue;
        }
        // Vector that will contain the command and the following arguments.
        std::vector<std::string> vec;
        // Stream that will extract arguments from a line and deposit them into
        // arg.
        std::stringstream ss(line);
        // String that will store the cmd, and any one argument to be put into
        // the vector.
        std::string arg;
        // Output that aligns with the output in the homework file.
        std::cout << "Running:";
        // While there are values on the line, it will put them into arg, one
        // at a time and push them into the vector.
        while (ss >> arg) {
            vec.push_back(arg);
            // Output that aligns with the output in the homework file.
            std::cout << " " << arg;
        }
        // Output that aligns with the output in the homework file.
        std::cout << std::endl;
        // Stores the childPid of the system call made in forkNexec.
        int childPid = forkNexec(vec);
        // Stores the childPid in a vector to call the waitpid method later.
        children.push_back(childPid);
    }
    // Runs all of the waitpid methods for each of the child processes in the 
    // order that they were called.
    for (auto c : children) {
        waitpid(c, &exitCode, 0);
        std::cout << "Exit code: " << exitCode << std::endl;
    }    
}
/**
 * Takes a command and its arguments as strings and runs it in a child process.
 * @param cmd Command that is to be run.
 * @param args Arguments that will used with the command.
 */
void cmdElse(std::string cmd, std::string args) {
    // Vector of strings that will store the command first, then the rest of 
    // the arguments to be used with that command. 
    std::vector<std::string> vec = {cmd};
    // Stream that will ease the extraction and insertion of data from args into
    // vec.
    std::stringstream ss(args);
    // Temporary string used to store values from args into vec via ss.
    std::string arg;
    // Output that aligns with the specified output in the homework file.
    std::cout << "Running: " << cmd;
    // Loop that takes the rest of the arguments separated by spaces in args
    // and puts them into arg--while removing quotes--as long as there are 
    // arguments to put into it.
    while (ss >> std::quoted(arg)) {
        // Adds an argument into the vector.
        vec.push_back(arg);
        // Output that aligns with the specified output in the homework file.
        std::cout << " " << arg;
    }
    // Output that aligns with the specified output in the homework file.
    std::cout << std::endl;
    // Stores the childPid of the system call made in forkNexec.
    int childPid = forkNexec(vec);
    // Exit code to be modified
    int exitCode = 0;
    // Waits for the childPid to be finished and modifies the exit code to 
    // the exit code of the child process.
    waitpid(childPid, &exitCode, 0);
    // Output that aligns with the specified output in the homework file. 
    std::cout << "Exit code: " << exitCode << std::endl;
}
/**
 * This method prompts the user for input and then determines what to do with 
 * the specified input. Either run a file serially, in parallel, exit the bash,
 * or run a command with arguments.
 */
void proc() {
    // String that will be used to store the input of the user.
    std::string line;
    // While loop that will continue prompting the user until they input "exit".
    while (std::cout << "> ", std::getline(std::cin, line)) {
        // String that will be used with the logic to determine if it is to be 
        // run serially, in parallel, or just a basic command with arguments.
        std::string cmd;
        // Stream that eases the process of extracting the cmd from the user 
        // input.
        std::stringstream in(line);
        // Takes the first word typed by the user and stores into cmd.
        in >> cmd;
        // Logical if/else branch to determine what to run(serial, parallel,
        // exit, standard command).
        if (cmd == "exit") {
            // Exits this method if the user types exit.
            return;
        } else if (cmd[0] == '#' || cmd == "") {
            // Skips to the next line if it is a comment or blank.
            continue;
        } else if (cmd == "SERIAL") {
            // If the first word is "SERIAL", then we declare another string
            // that the following word will be stored in. This represents a 
            // file that contains more code to be executed. 
            std::string filename;
            // Takes the next word of user input and stores into filename.
            in >> std::quoted(filename);
            // Method call to serial that will run the commands in the file
            // serially.
            serial(filename);
        } else if (cmd == "PARALLEL") {
            // If the first word is "PARALLEL", then we declare another string
            // that the following word will be stored in. This represents a 
            // file that contains more code to be executed. 
            std::string filename;
            // Takes the next word of user input and stores into filename.
            in >> std::quoted(filename);
            // Method call to parallel that will run the commands in the file
            // in parallel.
            parallel(filename);
        } else {
            // If it is none of the above special cases, we will take the first
            // word as a command, and the rest of the line as arguments.
            // String args is used to store the rest of the line.
            std::string args;
            std::getline(in, args);
            // Removes any quotes that we don't want to print.
            std::quoted(args);
            // Method call to cmdElse that will run the specified command with 
            // its supplemental arguments.
            cmdElse(cmd, args);
        }
    }
}

int main(int argc, char** argv) {
    // Runs the bash process and returns 0 if no errors are thrown.
    proc();
    return 0;
}

