#include <iostream>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

class FastReader {
	public:
		uintmax_t n_line = 0; // exclude 1st line
		uintmax_t read_file( const char* fname, bool hasheader = true );
		void write_file( string outfname );
		string getFName(void);
		void setDelimiter(const char* deli);		
	protected:
		const char* fname; // input file name and path
		struct LineStat {
			int min_pos;
			int max_pos;
		};
		unordered_map <string, LineStat> line_stat_map;
		string delimiter = "\t";		
		const char* map_file( const char* fname, size_t& length );
		void assign_line_stat_map( const char* start, const char* end );		
		void output( ofstream & myfile );
	private:
		void handle_error( const char* msg ); 
};

