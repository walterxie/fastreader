#include <algorithm>
#include <iostream>
#include <cstring>

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
		uintmax_t read_file( const char* fname );
		void write_file( string outfname );
		string getFName(void);
	protected:
		const char* fname; // input file name and path
		struct LineStat {
			int min_pos;
			int max_pos;
		};
		unordered_map <string, LineStat> line_stat_map;		
		const char* map_file( const char* fname, size_t& length );
		void assign_line_stat_map( const char* start, const char* end );		
		void output( ofstream & myfile );
	private:
		void handle_error( const char* msg ); 
};

string FastReader::getFName(void) {
    return string(fname);
}

//   
void FastReader::assign_line_stat_map(const char* start, const char* end) {
	char* ps = const_cast<char*>(start);
	char* pch = strchr(ps,'\t');
	string name = string(ps, pch);
	ps = pch;
	pch = strchr(pch+1,'\t');
	int p = atoi( ps );

	auto it = line_stat_map.find(name);
	if( it != line_stat_map.end() ) {
		LineStat& l_stat = it->second;
		if (l_stat.min_pos > p) 
			l_stat.min_pos = p;
		else if (l_stat.max_pos < p)
			l_stat.max_pos = p;
	} else {
		struct LineStat new_l_stat;
		new_l_stat.min_pos = p;
		new_l_stat.max_pos = p;
		line_stat_map[name] = new_l_stat;
	}
}   


void FastReader::handle_error(const char* msg) {
    perror(msg); 
    exit(255);
}

const char* FastReader::map_file(const char* fname, size_t& length) {
    int fd = open(fname, O_RDONLY);
    if (fd == -1)
        handle_error("open");

    // obtain file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    length = sb.st_size;

    const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED)
        handle_error("mmap");

    return addr;
}

uintmax_t FastReader::read_file( const char* fname ) {   
	this->fname = fname;

	size_t length;
    auto f = map_file(fname, length);    
    std::cout << "Open file : " << fname << std::endl;
    
    auto l = f + length;
    const char* end;

    // ignore 1st line, which is column names
    auto l1 = static_cast<const char*>(memchr(f, '\n', l-f));
    if (l1) {    
		std::cout << "Ignore 1st line : " << string(f, l1) << std::endl;
		f = l1 + 1;
	} else {
		perror("Invalid file format, cannot find '\n' !");
		handle_error("memchr");
	}
	
    uintmax_t n_line = 0;
    while (f && f<l) {
        end = static_cast<const char*>(memchr(f, '\n', l-f)); // end -> \n
        if (end) {           
            // process line f ... end-1
            assign_line_stat_map(f, end-1);
            f = end + 1;
            n_line++;
            // info every 1M lines 
            if (n_line % 100000000 == 0)
				std::cout << n_line / 1000000 << " millions lines processed" << std::endl;
        } else {
        	break;
        }
    }

    std::cout << "Total n_line = " << n_line << std::endl;    
    
    return n_line;
}

void FastReader::output( ofstream & myfile ) {
	for ( auto it = line_stat_map.begin(); it != line_stat_map.end(); ++it )
		myfile << it->first << "\t" << it->second.min_pos << "\t" << it->second.max_pos << std::endl; 
}

void FastReader::write_file( string outfname ) {   
	ofstream myfile;
	std::cout << "Write file : " << outfname << std::endl;
	myfile.open (outfname);
	output(myfile); 
	myfile.close();
}


//64 bits
int main(int argc, char* argv[]) {
    // Check the number of parameters
    if (argc < 2) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " <FILE NAME>" << std::endl;
        std::cerr << "64 bits" << std::endl;
        return 1;
    }
    
    FastReader fastReader;   
    
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
