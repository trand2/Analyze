// Project:  Analyze
// Name: Tyler Randolph
// Submitted: 10/28/2019
// I declare that the following source code was written by me, or provided
// by the instructor for this project. I understand that copying source
// code from any other source, providing source code to another student,
// or leaving my code on a public web site constitutes cheating.
// I acknowledge that  If I am found in violation of this policy this may result
// in a zero grade, a permanent record on file and possibly immediate failure of the class.

/*
 Reflection (1-2 paragraphs):  I really enjoyed writing this program, but had trouble finding the piggybacks. I realized I
 was not finding the peak of the pulse and only searching from where the pulse was found. Once I figured that out it
 wasn't too bad from there.
 */

// analyze.cpp: Data reduction on digitized samples
#include <algorithm>    // For for_each, transform, adjacent_find, count_if, copy, max_element, min
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

//    if (!in) {
//        cout << "File does not exist" << endl;
//        exit(1);
//    }
   
        try {
            //you will run the files this way getting file names from command line
            //parse_ini(ini_file);
            //analyze(data_file, cout);
            if (argc != 3) {
                throw invalid_argument("Invalid amount of files");
            }

            ini_file = argv[1];
            data_file = argv[2];

            parse_ini(argv[1]);
            analyze(data_file, cout);
            
            
            //my test cases will not use command line and I run them using the following commands
            // parse_ini("error1.ini");
            // parse_ini("error2.ini");
            // parse_ini("error3.ini");
            // parse_ini("error4.ini");
            //parse_ini("gage2scope.ini");   //this is the good .ini file
           // analyze("2_Record2308.dat",cout);
           // analyze("as_ch01-0537xx_Record1042.dat",cout);
            //analyze("2_Record3388.dat",cout);
        }
        catch (exception& x) {
            cerr << x.what() << '\n';
        }
    
//    cout << vt << " " << width << " "  << pulse_delta << " "  << drop_ratio << " "  << below_drop_ratio << "\n";
}

void analyze(const string& filename,ostream &os) {
   //implement your analyze code here
   //make sure you output to the 'os' stream passed into the function. that's how I test your output
    const int NUMBERS_SIZE = 3072;
    const int SMOOTH_SIZE = 3072;
    int numbers[NUMBERS_SIZE] = {0};
    int smooth[SMOOTH_SIZE] = {0};
    int potentialPulse;
    vector<int> pulses;
    vector<int> areaVector;
    string data;
    
    //Open file
    ifstream in(filename);
    if (!in) {
        throw invalid_argument("File does not exist");
    }
    
    os << filename << ":" << endl;
    
    //Parse .dat file
    int i = 0;
    while (in) {
        getline(in, data);
        if (data != "") {
            numbers[i] = stoi(data);
            i++;
        }
    }
    
    //Negate all values in array
    transform(begin(numbers), end(numbers), numbers, negate<int>());
    
    //copy the first 3 numbers from the original, negated data to the smooth array.
    copy(numbers, numbers + 3, begin(smooth));
    
/*
     Starting with the 4th point of the array and ending with the 4th from
     the last, replace each of those points with the following weighted average of its neighbors.
*/
    i = 3;
    for (int j = 3; j < NUMBERS_SIZE - 3; j++) {
        int smoothData = 0;
        smoothData = (numbers[j-3] + 2*numbers[j-2] + 3*numbers[j-1] + 3*numbers[j] + 3*numbers[j+1] + 2*numbers[j+2] + numbers[j+3]) / 15;
        smooth[i] = smoothData;
        i++;
    }
    
    //copy the last 3 numbers over to fill out the smoothed vector.
    copy(numbers + (NUMBERS_SIZE - 3), numbers + NUMBERS_SIZE, smooth + SMOOTH_SIZE - 3);

    //Find pulses
    for (int j = 0; j < SMOOTH_SIZE - 2; j++) {
        if (smooth[j+2] - smooth[j] > vt) {
            potentialPulse = j;
            pulses.push_back(potentialPulse);
            //cout << "Potential Pulse begins at position: " << potentialPulse << endl;
            j = j+2;
            while (smooth[j] <= smooth[j+1]) {
                j++;
            }
        }
    }
    
    //Remove piggyback pulses
    for (int j = 0; j < pulses.size() - 1; j++) {
        int peakStart = pulses[j];
        int nextPeakStart = pulses[j+1];
        int pointCounter = 0;
        int point = 0;
        int pulseIter = 2;
        double dropTimesPeak = 0.0;
        int peak = 0;
        
        // Check to see if the start of a pulse is followed by another pulse that starts within (<=) pulse_delta positions
        // of it.
        if (nextPeakStart - peakStart <= pulse_delta) {
            
            while (smooth[peakStart + pulseIter + 1] != smooth[nextPeakStart]) {
                if (smooth[peakStart + pulseIter] > smooth[peakStart + pulseIter + 1]) {
                    peak = smooth[peakStart + pulseIter];
                    point = peak;
                    break;
                }
                pulseIter++;
            }
            
            //pulseIter = 0;
            dropTimesPeak = drop_ratio * peak;
            
            // Find how many points between the peak of the first pulse and the start of the second pulse (non-inclusive;
            // only look at point strictly inside this interval) are strictly less than drop_ratio times the height of the
            // peak of the first pulse.
            while (point != smooth[nextPeakStart - 1]) {
                if(point < dropTimesPeak) {
                    pointCounter++;
                    if (pointCounter == below_drop_ratio) {
                        break;
                    }
                }
                pulseIter++;
                point = smooth[pulses[j] + pulseIter];
            }
            
            // If the number is greater than or equal to the below_drop_ratio, omit the first pulse from further
            // consideration (it is not a pulse of interest).
            if (pointCounter >= below_drop_ratio) {
                pulses.erase(pulses.begin() + j);
                j--;
            }
                    
        }
            
    }
    os << "pulses: ";
    for (int i = 0; i < pulses.size(); i++) {
        os << pulses[i];
        if (i != pulses.size()) {
            os << " ";
        }
    }
    
    os << endl;
    
    // Calculate the area of your remaining pulses.
    // The area is merely the sum of the values starting at the pulse start and going for width samples (another input
    // parameter), or until the start of the next pulse, whichever comes first. Use the original, unsmoothed data to
    // compute the area,  however. (The smooth data is just for detecting pulses.)
    
    
    for (int i = 0; i < pulses.size(); i++) {
        int nextPulseStart = 0;
        int area = 0;
        int counter = 0;
        int pulseStart = pulses[i];
        if (pulses.size() > 1) {
            nextPulseStart = pulses[i+1];
        }
        int point = pulseStart;
        
        while (counter < width && point != nextPulseStart) {
            
            area += numbers[point];
            counter++;
            point = pulseStart + counter;
            
        }
        
        areaVector.push_back(area);
        
    }
    
    os << "areas: ";
    
    for (int i = 0; i < areaVector.size(); i++) {
        os << areaVector[i];
        if (i != areaVector.size()) {
            os << " ";
        }
    }
    
    os << endl;
        
}


void parse_ini(const char* inifile) {
    //implement your .ini file parsing here
    //you will populate the globals declared at top of file (vt, width etc.)
    //feel free to use the bitset pattern you used in XML
    bool isVT = false;
    bool isWidth = false;
    bool isPulseData = false;
    bool isDropRatio = false;
    bool isBelowDropRatio = false;
    string data = "";
    string invalidFile = "Invalid INI file: missing one or more keys";
    
    //Open file
    ifstream in(inifile);
    if (!in) {
        throw invalid_argument("File does not exist");
    }
    
    //parse .ini file
    while (in) {
        getline(in, data, '=');
        
        if (data == "vt") {
            isVT = true;
            getline(in, data, '\r');
            if (data == "") {
                throw invalid_argument("Missing value for: vt");
            }
            try {
                vt = stoi(data);
            } catch (...) {
                throw invalid_argument("Invalid value type: vt");
            }
        } else if (data == "width") {
            isWidth = true;
            getline(in, data, '\r');
            if (data == "") {
                throw invalid_argument("Missing value for: width");
            }
            try {
                width = stoi(data);
            } catch (...) {
                throw invalid_argument("Invalid value type: width");
            }
        } else if (data == "pulse_delta") {
            isPulseData = true;
            getline(in, data, '\r');
            if (data == "") {
                throw invalid_argument("Missing value for: pulse_data");
            }
            try {
                pulse_delta = stoi(data);
            } catch (...) {
                throw invalid_argument("Invalid value type: pulse_data");
            }
        } else if (data == "drop_ratio") {
            isDropRatio = true;
            getline(in, data, '\r');
            if (data == "") {
                throw invalid_argument("Missing value for: drop_ratio");
            }
            try {
                drop_ratio = stod(data);
            } catch (...) {
                throw invalid_argument("Invalid value type: drop_ratio");
            }
        } else if (data == "below_drop_ratio") {
            isBelowDropRatio = true;
            getline(in, data, '\r');
            if (data == "") {
                throw invalid_argument("Missing value for: below_drop_ratio");
            }
            try {
                below_drop_ratio = stoi(data);
            } catch (...) {
                throw invalid_argument("Invalid value type: below_drop_ratio");
            }
        } else if (data != ""){
            throw invalid_argument("Invalid or duplicate INI key: " + data);
        }
        getline(in, data, '\n');
    }
    
    //Check to make sure all required values were provided
    if (!isVT || !isWidth || !isPulseData || !isDropRatio || !isBelowDropRatio)
        throw invalid_argument(invalidFile);
}
