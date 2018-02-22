#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include "library.h"

int main(int argc, char * argv[]) {

	if (argc != 4) {
        std::cout << "Usage: " << argv[0];
        std::cout << " <csv_file> <heapfile> <page_size>" << std::endl;
        return 0;
    }

    std::ifstream csv_file(argv[1]);
    FILE *heapfile = fopen(argv[2], "w");

    int page_size = atoi(argv[3]);

    if (heapfile == NULL) {
    	std::cout << "Unable to open heapfile" << std::endl;
    	return -1;
    } else if (!csv_file.is_open()) {
    	std::cout << "Unable to open csv file" << std::endl;
    	return -1;
    }

    std::string line;
    Page page;
    struct timeb time;
    ftime(&time);

    unsigned long time1 = time.time * 1000 + time.millitm;

	init_fixed_len_page(&page, page_size, RECORD_SIZE * V_SIZE);

	while (getline(csv_file, line)) {
		Record record;
		std::string fields;
		std::stringstream sline(line);
		char *v;

		//get all the field in the record
		while (getline(sline, fields, ',')) {
			v = (char *) calloc(V_SIZE + 1, 1);
			std::strncpy(v, fields.c_str(), V_SIZE);
			v[V_SIZE] = '\0';
			record.push_back((V) v);
		}
		int freeslot = fixed_len_page_freeslots(&(page));
		
		if (freeslot > 0) {
			//page not full
			add_fixed_len_page(&page, &record);

		} else {
			// page full add new page
			fwrite((const void *) page.data, page_size, 1, heapfile);
			init_fixed_len_page(&page, page_size, RECORD_SIZE * V_SIZE);
			add_fixed_len_page(&page, &record);
		}
		//write last page
		fwrite((const void *) page.data, page_size, 1, heapfile);
		ftime(&time);
		unsigned long time2 = time.time * 1000 + time.millitm;

		std::cout << "Time: " << time2 - time1 << "millisecond.\n";

		fclose(heapfile);
		csv_file.close();

		return 0;
	}

}