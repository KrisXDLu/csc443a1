#include <iostream>
#include <fstream>
#include <stdlib.h>     
#include <sstream>
#include <cstring>
#include <sys/timeb.h>
#include "library.h"

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 7) {
        cout << "Usage: select3 <colstore_name> <attribute_id> <attribute_id_return> <start> <end> <page_size>" << endl;
        return -1;
    }

    char *colstore_name = argv[1];

    // parallel operation two files open
    int attribute_id = atoi(argv[2]);
    if (attribute_id < 0 || attribute_id >= RECORD_SIZE) {
        cout << "Error: attribute_id: [" << attribute_id << "] isn't in range [0 - 99]\n";
        return -1;
    }

    int attr_id_return = atoi(argv[3]);
    if (attr_id_return < 0 || attr_id_return >= RECORD_SIZE) {
        cout << "Error: attribute_id_return: [" << attr_id_return << "] isn't in range [0 - 99]\n";
        return -1;
    }

    ostringstream colstore_heapfile_pathstream;
    colstore_heapfile_pathstream << colstore_name << "/" << attribute_id;
    const char* colstore_heapfile_path = colstore_heapfile_pathstream.str().c_str();

    FILE *colstore_heapfile_ptr = fopen(colstore_heapfile_path, "r+");
    if (!colstore_heapfile_ptr) {
        cout << "Error: failed to open the file" << colstore_heapfile_path << endl;
        return -1;
    }

    ostringstream colstore_heapfile_pathstream_return;
    colstore_heapfile_pathstream_return << colstore_name << "/" << attr_id_return;
    const char* colstore_heapfile_path_return = colstore_heapfile_pathstream_return.str().c_str();

    FILE *colstore_heapfile_return = fopen(colstore_heapfile_path_return, "r+");
    if (!colstore_heapfile_return) {
        cout << "Error: failed to open the file" << colstore_heapfile_path_return << endl;
        return -1;
    }

    string start = string(argv[4]);
    string end = string(argv[5]);
    int page_size = atoi(argv[6]);

    Heapfile heap_file;
    Heapfile heap_file_return;

    init_heapfile(&heap_file, page_size, colstore_heapfile_ptr);
    init_heapfile(&heap_file_return, page_size, colstore_heapfile_return);


    // start timer
    struct timeb time;
    ftime(&time);
    unsigned long time1 = time.time * 1000 + time.millitm;
    
    // two iterator for two heap file
    RecordIterator record_iterator(&heap_file);
    RecordIterator record_iterator_return(&heap_file_return);
    Record c_record, c_record_return, result_record;
    Record::const_iterator i, j, k;

    // loop through two heap files to get both records and compare with params 
    while(record_iterator.hasNext() && record_iterator_return.hasNext()){
        c_record = record_iterator.next();
        c_record_return = record_iterator_return.next();
        for (i = c_record.begin(), j = c_record_return.begin(); i != c_record.end(); ++i, ++j){
            if (string(*i) >= start && string(*i) <= end) {
    			cout << string(*j).substr(1, 5) << endl;
    		}
        }
    }

    // calculate total time
    ftime(&time);
    unsigned long time2 = time.time*1000 + time.millitm;
    unsigned long total_time = time2 - time1;
    std::cout << "TIME: " << total_time << " milliseconds\n";

    fclose(colstore_heapfile_ptr);
    fclose(colstore_heapfile_return);
    return 0;
}