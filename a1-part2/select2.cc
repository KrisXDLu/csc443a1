#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/timeb.h>
#include <sstream>
#include <cstring>
#include "library.h"

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 6) {
        cout << "Usage: select2  <colstore_name> <attribute_id> <start> <end> <page_size>" << endl;
        exit(EXIT_FAILURE);
    }

    char *colstore_name = argv[1];

    int attr_id = atoi(argv[2]);
    if (attr_id < 0 || attr_id >= RECORD_SIZE) {
        cout << "Error: attribute_id: [" << attr_id << "] isn't in range [0 - 99]\n";
        return -1;
    }

    ostringstream cs_hf_ps; //colstore_heapfile_pathstream
    cs_hf_ps << colstore_name << "/" << attr_id;
    const char* colstore_heapfile_path = cs_hf_ps.str().c_str();

    FILE *colstore_heapfile_ptr = fopen(colstore_heapfile_path, "r+");
    if (!colstore_heapfile_ptr) {
        cout << "Error: failed to open the file" << colstore_heapfile_path << endl;
        return -1;
    }
    
    //init heapfile
    Heapfile heap_file;
    init_heapfile(&heap_file, page_size, colstore_heapfile_ptr);

    std::string start = std::string(argv[3]);
    std::string end = std::string(argv[4]);
    int page_size = atoi(argv[5]);


    // start timer
    struct timeb time;
    ftime(&time);
    unsigned long time1 = time.time * 1000 + time.millitm;

    RecordIterator record_iterator(&heap_file);

    Record c_record;
    Record::const_iterator i, j;

    // loop through heap file to get the record
    while(record_iterator.hasNext()){
        c_record = record_iterator.next();
        for (i = c_record.begin(); i != c_record.end(); ++i){
            if (string(*i) >= start && string(*i) <= end) {
    			cout << string(*i).substr(1, 5) << endl;
    		}
        }
    }


    // calculate total time
    ftime(&time);
    unsigned long time2 = time.time*1000 + time.millisec;
    unsigned long total_time = time2 - time1;
    std::cout << "TIME: " << total_time << " milliseconds\n";

    fclose(colstore_heapfile_ptr);
}