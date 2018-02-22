#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include "library.h"

int main(int argc, char *argv[]){

    if (argc != 3) {
        std::cout << "Usage: " << argv[0];
        std::cout << " <heapfile> <page_size>" << std::endl;
        return 0;
    }

    FILE *heapfile = fopen(argv[1], "r");
    int page_size = atoi(argv[2]);

    // I/O errors
    if (heapfile == NULL) {
        std::cout << "Unable to open the heapfile\n";
        return -1;
    }

    Page page;
    unsigned int j;
    init_fixed_len_page(&page, page_size, RECORD_SIZE * V_SIZE);

    // Read all pages from heapfile
    while (fread(page.data, page_size, 1, heapfile) > 0){
    	int capacity = fixed_len_page_capacity(&page);
    	for (int i = 0; i < capacity; ++i) {
    		Record record;
    		read_fixed_len_page(&page, i, &record);
            //display record
            int recordlen = fixed_len_sizeof(&record);
    		if (recordlen > 0) {
    			for (j = 0; j < record.size() - 1; ++j) {
                	std::cout << record.at(j) << ", ";
            	}
            	std::cout << record.at(j) << '\n';
    		}
    	}
    }
    return 0;
}