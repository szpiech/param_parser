#include "param_t.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

    param_t params;

    params.setPreamble("This is the preamble.\nUse it for messages in help.\n");

    params.addFlag("--boolFlag", false, "boolLabel", "A test bool flag.");
    params.addFlag("--doubleFlag", 0.0, "boolLabel", "A test bool flag.");
    params.addFlag("--intFlag", 0, "boolLabel", "A test bool flag.");
    params.addFlag("--charFlag", 'c', "boolLabel", "A test bool flag.");
    params.addFlag("--stringFlag", "string", "boolLabel", "A test bool flag.");
    params.addFlag("--silentFlag", "string", "SILENT", "This flag won't be in --help.");
    params.addListFlag("--stringList", "stringList", "", "A test string list flag.");
    params.addListFlag("--intList",0,"","A test int list flag.");

    try
    {
        params.parseCommandLine(argc, argv);
    }
    catch (...)
    {
        return 1;
    }

    cerr << "--boolFlag " << params.getBoolFlag("--boolFlag") << endl;
    cerr << "--doubleFlag " << params.getDoubleFlag("--doubleFlag") << endl;
    cerr << "--intFlag " << params.getIntFlag("--intFlag") << endl;
    cerr << "--charFlag " << params.getCharFlag("--charFlag") << endl;
    cerr << "--stringFlag " << params.getStringFlag("--stringFlag") << endl;
    cerr << "--silentFlag " << params.getStringFlag("--silentFlag") << endl;
    cerr << "--stringList ";

    vector<string> list = params.getStringListFlag("--stringList");

    for(int i = 0; i < list.size(); i++)
    {
      cerr << list[i] << " ";
    }
    cerr << endl;

    cerr << "--intList ";

    vector<int> list2 = params.getIntListFlag("--intList");

    for(int i = 0; i < list2.size(); i++)
    {
      cerr << list2[i] << " ";
    }
    cerr << endl;

    return 0;
}
