
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

/**
 * populate a random array (which is already
 * allocated with enough memory to hold n bytes.
 */
void random_array(char *array, long bytes) {
	for (int i = 0; i < bytes; i++) {
		array[i] = 'A' + (rand() % 26);
	}
} 

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: create_random_file <filename> <total bytes> <block size>\n");
	}
	// arguments
	char * filename = argv[1];
	long total_bytes = strtol(argv[2], NULL, 10);
	long block_size = strtol(argv[3], NULL, 10);
	
	long bytes_written = 0;
	long total_written = 0;
	long msec = 0;
	struct timeb time;
	char *buffer = malloc(sizeof(char) * total_bytes);

	random_array(buffer, total_bytes);

	FILE *fptr = fopen(filename, "w");

	if (fptr == NULL) {
		printf("Open file unsuccessful.\n");
		return -1;
	}

	do {
		//get read starting time
		ftime(&time);
		long time_before = time.time * 1000 + time.millitm;

		//write
		bytes_written = fwrite(buffer, 1, block_size, fptr);
		fflush(fptr);

		ftime(&time);
		long time_after = time.time * 1000 + time.millitm;

		msec += time_after - time_before;
		total_written += bytes_written;

	} while (total_written < total_bytes);

	free(buffer);
	printf("Block size %lu bytes\n", block_size);
	printf("Total bytes %lu bytes\n", total_bytes);
	printf("Time %lu millisec\n", msec);

	fclose(fptr);
	return 0;


}