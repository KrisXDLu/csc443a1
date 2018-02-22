#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <cstdio>
#include <sys/timeb.h>
#include "library.h"

int main(int argc, char* argv[]) {

	if (argc != 3) {
        std::cout << "Usage: " << argv[0];
        std::cout << " <page_file> <page_size>" << std::endl;
        return 0;
    }

	std::ifstream page_file(argv[1], std::ios::out| std::ios::binary);

	Page page;
	Record record;

	int page_size = atoi(argv[2]);
	int page_num = 0;
	int record_num = 0;
	int page_file_size;
	int slot_size;

	// Check if page file is opened for read
    if (!page_file.is_open()) {
    	std::cout << "Unable to open page file" << std::endl;
    	return 0;
    }

	struct timeb time;
	ftime(&time);
	unsigned long time1 = time.time * 1000 + time.millitm;

    while (!page_file.fail() && !page_file.bad()) {
    	++page_num;

    	page_file.read((char *) &page_file_size, sizeof(int));

    	if (page_file_size != page_size) {
    		std::cout << "Page size in the page file does not ";
            std::cout << "equal to the <page_size> given" << std::endl;
            return 0;
    	}

    	page_file.read((char *) &slot_size, sizeof(int));
    	init_fixed_len_page(&page, page_size, slot_size);
    	page_file.read((char *) page.data, page_size);
    	
    	//check end of file
    	if (page_file.eof()){
    		ftime(&time);
    		unsigned long time2 = time.time * 1000 + time.millitm;
    		std::cout << "Time: " << time2 - time1 << "millisecond.\n";
    		page_file.close();
    		return 0;
    	}
    	unsigned int j;
    	int capacity = fixed_len_page_capacity(&page);
    	for (int i = 0; i < capacity; ++i) {
    		Record record;
    		read_fixed_len_page(&page, i, &record);
    		for (j = 0; j < record.size() - 1; ++j) {
    			std::cout << record.at(j) << ", ";
    		}
    		std::cout << record.at(j) << std::endl;
    	}
    }
    std::cout << "Unable to read page file" << std::endl;

	return -1;	
}