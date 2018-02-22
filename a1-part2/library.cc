#include <iostream>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include "library.h"


/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record){
	Record::const_iterator i;

    // The total bytes read 
    int total = 0;

    // Iterate through elements in vector
    for (i = record->begin(); i != record->end(); ++i){
        total += strlen(*i) * sizeof(char);
    }
    return total;
}

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf) {
	Record::const_iterator i;
	// total written
	int total = 0;
	int size;

	for (i = record->begin(); i != record->end(); ++i) {
		size = strlen(*i);
		char *pointer = (char *) *i;
		memcpy(buf, *i, size);

		//update pointer
		buf = static_cast<char*>(buf) + size;
	}
}

/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record *record) {
	// total read
	int total = 0;

	while (total < size) {
		char * temp = (char *) malloc(sizeof(V_SIZE) + 1);
		// copy to temp
		memcpy(temp, buf, V_SIZE);
		temp[V_SIZE] = '\0';
		// push to record
		record->push_back((V) temp);
		buf = static_cast<char*>(buf) + V_SIZE;
		total += V_SIZE;
	}
}


/**
 * Initializes a page using the given slot size
 */
void init_fixed_len_page(Page *page, int page_size, int slot_size) {
	page->data = calloc(page_size, 1);
	page->page_size = page_size;
	page->slot_size = slot_size;
}

 
/**
 * Calculates the maximal number of records that fit in a page
 */
int fixed_len_page_capacity(Page *page) {
	return page->page_size / page->slot_size;
}
 
/**
 * Calculate the free space (number of free slots) in the page
 */
int fixed_len_page_freeslots(Page *page) {
	int capacity = fixed_len_page_capacity(page);
	int used = 0;

	for (int i = 0; i < capacity; i++) {
		int offset = i * page->slot_size;
		int size = strlen(static_cast<char*>(page->data) + offset);
		used += (size > 0) ? 1:0;
	}
	return capacity - used;
}
 
/**
 * Add a record to the page
 * Returns:
 *   record slot offset if successful,
 *   -1 if unsuccessful (page full)
 */
int add_fixed_len_page(Page *page, Record *r) {
	int capacity = fixed_len_page_capacity(page);
	for (int i = 0; i < capacity; i++) {
		int offset = i * page->slot_size;
		char* loc = static_cast<char*>(page->data) + offset;
		if (strlen(loc) == 0) {
			fixed_len_write(r, (void *) loc);
			return i;
		}
	}
	return -1;
}
 
/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r) {
	int size = fixed_len_sizeof(r);
	void* buffer = calloc(size, 1);
	fixed_len_write(r, buffer);
	int offset = page->slot_size * slot;
	char* pointer = static_cast<char*>(page->data) + offset;
	memcpy(pointer, buffer, page->slot_size);
	free(buffer);
}

/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r) {
	int offset = page->slot_size * slot;
	void* buffer = calloc(page->slot_size, 1);
	char * pointer = static_cast<char*>(page->data) + offset;
	memcpy(buffer, pointer, page->slot_size);
	fixed_len_read(buffer, page->slot_size, r);
	free(buffer);
}


/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file) {
	heapfile->file_ptr = file;
    heapfile->page_size = page_size;
}

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile) {
	void * empty = calloc(heapfile->page_size, 1);
	PageID id = 0;

	fseek(heapfile->file_ptr, 0, SEEK_SET);

	while (fgetc(heapfile->file_ptr) != EOF) {
		fseek(heapfile->file_ptr, heapfile->page_size, SEEK_CUR);
		id++;
	}
	fwrite(empty, heapfile->page_size, 1, heapfile->file_ptr);
	free(empty);
	return (PageID) id;
}

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page) {
	int offset = heapfile->page_size * pid;
	fseek(heapfile->file_ptr, offset, SEEK_SET);
	fread(page->data, heapfile->page_size, 1, heapfile->file_ptr);
}

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid) {
	int offset = heapfile->page_size * pid;
	fseek(heapfile->file_ptr, offset, SEEK_SET);
	fwrite(page->data, heapfile->page_size, 1, heapfile->file_ptr);
}

RecordIterator::RecordIterator(Heapfile *heapfile){
    heap_file = heapfile;
    record_id.page_id = 0;
    record_id.slot = 0;
    fseek(heapfile->file_ptr, 0, SEEK_SET);
    init_fixed_len_page(&page, heap_file->page_size, RECORD_SIZE*V_SIZE);
    read_page(heap_file, record_id.page_id, &page);
}

Record RecordIterator::next() {
    Record record;
    read_fixed_len_page(&page, record_id.slot++, &record);
    return record;
}

bool RecordIterator::hasNext() {

    int freespace = fixed_len_page_freeslots(&page);
    int capacity = fixed_len_page_capacity(&page);

    // check if we have already scan the entire page
    if (capacity - record_id.slot == freespace){
    	//haven't scaned the page

        // check if offset would exceed file boundary
        ++record_id.page_id;
        int offset = heap_file->page_size * record_id.page_id;
        fseek(heap_file->file_ptr, offset, SEEK_SET);
        long next_page_position = ftell(heap_file->file_ptr);
        fseek(heap_file->file_ptr, -1, SEEK_END);
        long end_of_file_position = ftell(heap_file->file_ptr);
        if(next_page_position > end_of_file_position)
            return false;

        read_page(heap_file, record_id.page_id, &page);

        // totally empty page which means we should stop reading, don't waste time checking following pages which are full of NULL
        if (strlen((char *) page.data) == 0) {
            return false;
        }

        record_id.slot = 0;
        
    }
        
    return true;
}


