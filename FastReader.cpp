#include <string.h>
#include "FastReader.h"
#include "str_search.h"

FastReader::FastReader() {
	n_line = 0; 
	delimiter = "\t";
}

string FastReader::getFName(void) {
    return string(fname);
}

void FastReader::setDelimiter(const char* deli) {
    this->delimiter=string(deli);
}

//   
void FastReader::assign_line_stat_map(const char* start, const char* end) {
	char* ps = const_cast<char*>(start);
	char* pch = mem_mem(ps, end, delimiter.c_str(), delimiter.length());
	string name = string(ps, pch);
	ps = pch;
	pch = mem_mem(pch+1, end, delimiter.c_str(), delimiter.length());
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

uintmax_t FastReader::read_file( const char* fname, bool hasheader /* = true */ ) {   
	this->fname = fname;

	size_t length;
    auto lineBegin = map_file(fname, length);
    std::cout << "Open file : " << fname << std::endl;
    
    auto l = lineBegin + length;
    const char* end;

    if (hasheader) {
		// ignore 1st line, which is column names
		auto l1 = static_cast<const char*>(memchr(lineBegin, '\n', l - lineBegin));
		if (l1) {    
			std::cout << "Ignore 1st line : " << string(lineBegin, l1) << std::endl;
			lineBegin = l1 + 1;
		} else {
			perror("Invalid file format, cannot find '\n' !");
			handle_error("memchr");
		}
	}
	
    uintmax_t n_line = 0;
    while (lineBegin && lineBegin<l) {
        end = static_cast<const char*>(memchr(lineBegin, '\n', l-lineBegin)); // end -> \n

        if (end) {           
            // process line lineBegin ... end-1
            assign_line_stat_map(lineBegin, end-1);
            lineBegin = end + 1;
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
		myfile << it->first << delimiter << it->second.min_pos << delimiter << it->second.max_pos << std::endl; 
}

void FastReader::write_file( string outfname ) {   
	ofstream myfile;
	std::cout << "Write file : " << outfname << std::endl;
	myfile.open (outfname);
	output(myfile); 
	myfile.close();
}



