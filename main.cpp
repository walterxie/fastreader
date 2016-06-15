
#include <iostream>
#include <string>
#include "FastReader.h"
using namespace std;

// The 1st version will consume more memory as file size increase
int main(int argc, char* argv[]) {
    // Check the number of parameters
    if (argc < 2) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " <FILE NAME>" << std::endl;
        std::cerr << "    or " << argv[0] << " <FILE NAME> <delimiter>" << std::endl;
        std::cerr << "64 bits OS required" << std::endl;
        return 1;
    }
    
    FastReader fastReader;   
    
    if (argc == 3) {
		fastReader.setDelimiter(argv[2]);
		std::cout << "Set delimiter between values to : value1" << argv[2] << "value2" << std::endl;
    }
    
//     uintmax_t n_line = fastReader.read_file(argv[1]);
	fastReader.read_file(argv[1]);
    
    string filename = fastReader.getFName();
    // remove .pileup_summarize from file name
    string rmsubstr = ".pileup_summarize";
    string::size_type i = filename.find(rmsubstr);
	if (i != string::npos)
		filename.erase(i, rmsubstr.length());
    // take only file name, to save output file in current directory
    i = filename.find_last_of("/"); 
    if (i != string::npos)
		filename = filename.substr(i+1, filename.length());
    // add postfix
    i = filename.find_last_of("."); 
	string outfname = filename.substr(0, i) + ".position.txt"; 

	fastReader.write_file( outfname );

    return 0;
}
