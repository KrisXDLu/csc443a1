CC = g++

all: write_fixed_len_pages  read_fixed_len_page csv2heapfile scan insert update delete select

library.o: library.cc library.h
	$(CC) -o $@ -c $<

write_fixed_len_pages: write_fixed_len_pages.cc library.o
	$(CC) -o $@ $< library.o

read_fixed_len_page: read_fixed_len_page.cc library.o
	$(CC) -o $@ $< library.o

csv2heapfile: csv2heapfile.cc library.o
	$(CC) -o $@ $< library.o

scan: scan.cc library.o
	$(CC) -o $@ $< library.o

insert: insert.cc library.o
	$(CC) -o $@ $< library.o

update: update.cc library.o
	$(CC) -o $@ $< library.o

delete: delete.cc library.o
	$(CC) -o $@ $< library.o

select: select.cc library.o
	$(CC) -o $@ $< library.o