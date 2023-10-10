//
// File: unit_test_command_line_argument_parser.cpp
// Date: 19 July 2023
// Author: TQ Smith
// Principal Investigator: Dr. Zachary Szpiech
// Purpose: Unit test the command line argument parser.
//

#include "../CommandLineArgumentParser.hpp"

#include <cassert>

int main() {

    cout << endl;
    cout << "Testing the Command Line Argument Parser!!!" << endl;
    cout << endl;

    cout << "Creating the parser object..." << endl;
    CommandLineArgumentParser parser;
    cout << "Created the parser object!" << endl << endl;

    bool successfulOperation;

    // Test the addOption method.
    cout << "Now, we test the addOption method..." << endl;

    // Make sure you cannot use the empty string as an option.
    parser.addOption("", "Just a test option 1.", &successfulOperation);
    assert(!successfulOperation);

    // Make sure you can add an option.
    parser.addOption("test1", "Just a test option 1.", &successfulOperation);
    assert(successfulOperation);

    // Make sure you cannot add a duplicate option.
    parser.addOption("test1", "Just a test option 1.", &successfulOperation);
    assert(!successfulOperation);

    // Add another option.
    parser.addOption("t2", "Just a test option 2.", &successfulOperation);
    assert(successfulOperation);
    cout << "We have succesfully added two options!" << endl << endl;

    // Test the printOptionDescription method.
    cout << "We print the description of the options..." << endl;
    parser.printOptionDescriptions();
    cout << endl << endl;

    // Test the parseCommandLine method.
    cout << "Now, we parse the commandline arguments..." << endl;
    cout << "First, we throw errors." << endl;
    char *argv1[2] = {"test", "--dne"};

    // Make sure you cannot parse an option that does not exist.
    parser.parseCommandLine(2,  argv1, &successfulOperation);
    assert(!successfulOperation);
    char *argv2[3] = {"test", "--test1", "--test1"};

    // Make sure you cannot parse the same option twice.
    parser.parseCommandLine(3,  argv2, &successfulOperation);
    assert(!successfulOperation);
    char *argv3[2] = {"test", "dne"};

    // Make sure the first option is indeed an option.
    parser.parseCommandLine(2,  argv3, &successfulOperation);
    assert(!successfulOperation);
    cout << endl;
    
    int num_arguments;
    int* int_args = NULL;
    double* double_args = NULL;
    string* string_args = NULL;

    // Test getOptionArguments method.
    cout << "Second, we test successful parsing and move to testing the getOptionArguments procedure." << endl;
    CommandLineArgumentParser parser2;
    parser2.addOption("test1", "Just a test option 1.", &successfulOperation);
    parser2.addOption("t2", "Just a test option 2.", &successfulOperation);
    parser2.addOption("t3", "Just a test option 3.", &successfulOperation);
    parser2.addOption("help",  "Prints the preamble and options list for the program.", &successfulOperation);
    char *argv4[8] = {"test", "--test1", "1", "2", "--help", "--t2", "0.01", "ab"};
    parser2.parseCommandLine(8,  argv4, &successfulOperation);
    assert(successfulOperation);

    // We test the getNumberOfArguments function.
    assert(parser2.getNumberOfArguments("test1", &successfulOperation) == 2);
    assert(parser2.getNumberOfArguments("help", &successfulOperation) == 0);
    assert(parser2.getNumberOfArguments("t2", &successfulOperation) == 2);
    assert(parser2.getNumberOfArguments("t3", &successfulOperation) == -1);
    parser2.getNumberOfArguments("invalid", &successfulOperation);
    assert(!successfulOperation);

    // Make sure you cannot get the value of an option that does not exist.
    int_args = parser2.getOptionArguments<int>("--dne", &num_arguments, &successfulOperation);
    assert(!successfulOperation && int_args == NULL);

    // Make sure you can get an option exists but was not set.
    int_args = parser2.getOptionArguments<int>("--t3", &num_arguments, &successfulOperation);
    assert(successfulOperation && int_args == NULL && num_arguments == -1);

    // Make sure you can get an option that exits, was set, but has not arguments.
    int_args = parser2.getOptionArguments<int>("--help", &num_arguments, &successfulOperation);
    assert(successfulOperation && int_args == NULL && num_arguments == 0);

    // Make sure flag is set if type cast is invalid.
    int_args = parser2.getOptionArguments<int>("--t2", &num_arguments, &successfulOperation);
    assert(!successfulOperation && int_args == NULL);

    // Test successful integer arguments.
    int_args = parser2.getOptionArguments<int>("--test1", &num_arguments, &successfulOperation);
    assert(successfulOperation && int_args != NULL && num_arguments == 2);
    cout << "The arguments:" << endl;
    for (int i = 0; i < num_arguments; i++) {
        cout << int_args[i] << endl;
    }
    delete [] int_args;

    // Test successful double arguments.
    double_args = parser2.getOptionArguments<double>("--test1", &num_arguments, &successfulOperation);
    assert(successfulOperation && int_args != NULL && num_arguments == 2);
    cout << "The arguments:" << endl;
    for (int i = 0; i < num_arguments; i++) {
        cout << double_args[i] << endl;
    }
    delete [] double_args;

    // Test successful string arguments.
    string_args = parser2.getOptionArguments<string>("--t2", &num_arguments, &successfulOperation);
    assert(successfulOperation && int_args != NULL && num_arguments == 2);
    cout << "The arguments:" << endl;
    for (int i = 0; i < num_arguments; i++) {
        cout << string_args[i] << endl;
    }
    delete [] string_args;

    return 0;

}