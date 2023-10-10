//
// File: CommandLineArgumentParser.hpp
// Date: 19 July 2023
// Author: TQ Smith
// Principal Investigator: Dr. Zachary Szpiech
// Purpose: A simple class for parsing command line arguments 
//              supplied by the user.
//

#ifndef _COMMAND_LINE_ARGUMENT_PARSER_HPP_
#define _COMMAND_LINE_ARGUMENT_PARSER_HPP_

#include <string>
#include <sstream>
#include <list>
#include <map>
#include <iostream>

using namespace std;

class CommandLineArgumentParser {

    public:

        // Add an option for the command line.
        // Accepts:
        //  string option -> The name of the option to add.
        //  string description -> The decription of the option.
        //  bool* successfulOperation -> Sets boolean if operation was valid.
        // Returns: void.
        void addOption(string option, string description, bool* successfulOperation);

        // Return an array containing the arguments supplied to an option.
        // Accepts:
        //  string option -> The name of the option.
        //  int* num_arguments -> Sets the number of arguments supplied by the user.
        //  bool* successfulOperation -> Sets boolean if operation was valid.
        // Returns: T*, a pointer to an num_arguments-element array of arguments of type T.
        template <typename T>
        T* getOptionArguments(string option, int* num_arguments, bool* successfulOperation);

        // Parsers command line arguments as supplied by the user.
        // Accepts:
        //  int argc -> The number of arguments.
        //  char *argv[] -> The arguments.
        //  bool* successfulOperation -> Sets boolean if operation was valid.
        // Returns: void.
        void parseCommandLine(int argc, char *argv[], bool* successfulOperation);

        // Tests if option has been set by the user.
        // Accepts:
        //  string option -> The name of the option.
        //  bool* successfulOperation -> Sets boolean if operation was valid.
        // Returns: int, The number of arguments supplied by the user.
        int getNumberOfArguments(string option, bool* successfulOperation);

        // Prints the description for each option.
        // Accepts: void.
        // Returns: void.
        void printOptionDescriptions();

        ~CommandLineArgumentParser();

    private:

        // Represents an option
        struct Option {
            // Number of arguments supplied.
            int num_arguments;
            // Description of the option.
            string description;
            // List of arguments supplied by the user.
            list<string> option_arguments;
        };

        // Maps option name to its attributes.
        map<string, Option*> arguments;

};


// A helper function to form an option from an option name.
// Accepts:
//  string option -> The name of the option.
// Returns: string, the formatted option.
string form_option(string option) {
    if (option.length() == 1) {
        return "-" + option;
    }
    return "--" + option;
}

void CommandLineArgumentParser::addOption(string option, string description, bool* successfulOperation) {

    // Do not allow the empty string as an option.
    if (option.length() == 0) {
        cerr << "Cannot use the empty string as an option!" << endl;
        *successfulOperation = false;
        return;
    }

    // Form the option.
    option = form_option(option);

    // Do not allow adding an option that already exists.
    if (arguments.count(option)) {
        cerr << "Option " << option << " already exists!" << endl;
        *successfulOperation = false;
        return;
    }

    // Set default values for the option.
    Option* temp = new Option;
    temp -> num_arguments = -1;
    temp -> description = description;

    // Add option to dictonary.
    arguments[option] = temp;

    *successfulOperation = true;

}

void CommandLineArgumentParser::parseCommandLine(int argc, char *argv[], bool* successfulOperation) {

    // Pointer to current argument.
    Option* opt = NULL;

    string option;

    // Iterate through user entered tokens from the command line.
    for (int i = 1; i < argc; i++) {

        // If an option is encountered.
        if (argv[i][0] == '-') {

            option = argv[i];

            // Make sure the option exits in the dictionary.
            if (!arguments.count(option)) {
                cerr << "Option " << option << " does not exists!" << endl;
                *successfulOperation = false;
                return;
            }

            // Get the option.
            opt = arguments[option];

            // Make sure it has not already been used.
            if (opt -> num_arguments != -1) {
                cerr << "Option " << option << " was already used!" << endl;
                *successfulOperation = false;
                return;
            }

            // No arguments have been passed.
            opt -> num_arguments = 0;

        // If the token is not an option.
        } else {
            
            // Make sure the first argument is an option.
            if (opt == NULL) {
                cerr << "No option created for " << argv[i] << "." << endl;
                *successfulOperation = false;
                return;
            }

            // Add argument to current option's argument list
            //  and increment the number of arguments.
            opt -> option_arguments.push_back(string(argv[i]));
            opt -> num_arguments++;

        }

    }

    *successfulOperation = true;

}

template <typename T>
T* CommandLineArgumentParser::getOptionArguments(string option, int* num_arguments, bool* successfulOperation) {

    // Make sure the argument exists.
    if (!arguments.count(option)) {
        cerr << "Option " << option << " does not exists!" << endl;
        *successfulOperation = false;
        return NULL;
    }

    // Get the option.
    Option* opt = arguments[option];

    // If the option was not set or no arguments were supplied,
    //  then we do not have to parse any arguments.
    if (opt -> num_arguments == -1 || opt -> num_arguments == 0) {
        *num_arguments = opt -> num_arguments;
        *successfulOperation = true;
        return NULL;
    }

    // Create array of arguments.
    T* args = new T[opt -> num_arguments];
    
    int index = 0;
    for (list<string>::iterator it = opt -> option_arguments.begin(); it != opt -> option_arguments.end(); it++) {
        istringstream strstream_arg(*it);
        // Convert each argument to the type.
        //  If conversion is not valid, then throw error.
        if (!(strstream_arg >> args[index])) {
            cerr << "Cannot perform type cast of argument for option " << option << "!" << endl;
            delete[] args;
            *successfulOperation = false;
            return NULL;
        }
        index++;
    }

    *num_arguments = opt -> num_arguments;

    *successfulOperation = true;

    return args;

}

int CommandLineArgumentParser::getNumberOfArguments(string option, bool* successfulOperation) {

    option = form_option(option);

    // Make sure the argument exists.
    if (!arguments.count(option)) {
        cerr << "Option " << option << " does not exists!" << endl;
        *successfulOperation = false;
        return -1;
    }

    // Return the number of arguments.
    return arguments[option] -> num_arguments;
}

void CommandLineArgumentParser::printOptionDescriptions() {

    // Iterate through options and print  description.
    for (map<string, Option*>::iterator it = arguments.begin(); it != arguments.end(); it++) {
        cout << it -> first << " " << it -> second -> description << endl;
    }

}

CommandLineArgumentParser::~CommandLineArgumentParser() {

    // Iterate through options and delete option structure.
    for (map<string, Option*>::iterator it = arguments.begin(); it != arguments.end(); it++) {
        delete (it -> second);
    }

}

#endif