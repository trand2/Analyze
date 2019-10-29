// analyze.cpp: Data reduction on digitized samples
#include <algorithm>    // For for_each, transform, adjacent_find, count_if,
                        // copy, max_element, min
#include <bitset>
#include <cctype>       // For tolower
#include <cstdio>       // For remove
#include <cstdlib>      // For system
#include <fstream>
#include <functional>   // For bind
#include <iostream>
#include <iterator>     // For back_inserter, ostream_iterator, istream_iterator
#include <sstream>
#include <numeric>      // For accumulate
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

// System Parameters
int vt,width,pulse_delta,below_drop_ratio;
double drop_ratio;

// Forward declarations
void parse_ini(const char*);
void analyze(const string& filename,ostream &os);

int main(int argc, char* argv[]) {
   
   string ini_file;
   string data_file;
   //get these from command line
   
        try {
            //you will run the files this way getting file names from command line
            //parse_ini(ini_file);
            //analyze(data_file, cout);
            
            //my test cases will not use command line and I run them using the following commands
            // parse_ini("error1.ini");
            // parse_ini("error2.ini");
            // parse_ini("error3.ini");
            // parse_ini("error4.ini");
           // parse_ini("gage2scope.ini");   //this is the good .ini file
           // analyze("2_Record2308.dat",cout);
           // analyze("as_ch01-0537xx_Record1042.dat",cout);
            //analyze("2_Record3388.dat",cout);
        }
        catch (exception& x) {
            cerr << x.what() << '\n';
        }
}

void analyze(const string& filename,ostream &os) {
   //implement your analyze code here
   //make sure you output to the 'os' stream passed into the function. that's how I test your output
}


void parse_ini(const char* inifile) {
    //implement your .ini file parsing here
    //you will populate the globals declared at top of file (vt, width etc.)
    //feel free to use the bitset pattern you used in XML
    }
