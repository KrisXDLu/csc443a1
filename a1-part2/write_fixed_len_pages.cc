#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <sys/timeb.h>
#include "library.h"

int main(int argc, char*argv[]) {
	if (argc != 4) {
        std::cout << "Usage: " << argv[0];
        std::cout << " <csv_file> <page_file> <page_size>" << std::endl;
        return 0;
    }
    std::ifstream csv_file(argv[1]);
    std::ofstream page_file(argv[2], std::ios::out | std::ios::binary);
    int page_size = atoi(argv[3]);

    std::string line;
    Page page;

    int rec_num = 0;
    int page_num = 0;

    struct timeb time;
    ftime(&time);

    unsigned long time1 = time.time*1000 + time.millitm;
    
    init_fixed_len_page(&page, page_size, RECORD_SIZE * V_SIZE);

    if (csv_file.is_open()) {
    	while (getline(csv_file, line)) {
    		Record record;
    		std::string field;
    		std::stringstream sline(line);
    		char *v;

    		// get all field
    		while (getline(sline, field, ',')) {
    			v = (char*) calloc(V_SIZE + 1, 1);
    			std::strncpy(v, field.c_str(), V_SIZE);
    			v[V_SIZE] = '\0';
    			record.push_back((V) v);
    		}

    		if (fixed_len_page_freeslots(&page) > 0) {
    			//page not full
    			add_fixed_len_page(&page, &record);
    		} else {
    			page_file.write((const char *) &(page.page_size), sizeof(int));
    			page_file.write((const char *) &(page.slot_size), sizeof(int));
    			page_file.write((const char *) page.data, page.page_size);
    			++page_num;
    			printf("%d %d\n", page.slot_size, V_SIZE*RECORD_SIZE);
    			init_fixed_len_page(&page, page.page_size, page.slot_size);
    			add_fixed_len_page(&page, &record);
    		}
    		++rec_num;
    	}
    	page_file.write((const char *) &(page.page_size), sizeof(int));
    	page_file.write((const char *) &(page.slot_size), sizeof(int));
    	page_file.write((const char *) page.data, page.page_size);
    	++page_num;
    	ftime(&time);
    	unsigned long time2 = time.time*1000 + time.millitm;
    	page_file.close();
    	csv_file.close();
    
    	std::cout << "number of records: " << rec_num << std::endl;
        std::cout << "number of pages: " << page_num << std::endl;
        std::cout << "time: " << time2 - time1 << " milliseconds\n";

    } else {
        std::cout << "CSV file open failed" << std::endl;;
    }

    return 0;
}