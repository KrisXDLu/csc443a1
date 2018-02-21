
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <assert.h>
/**
 * file_ptr : the file pointer, ready to be read from.
 * hist: an array to hold 26 long integers.  hist[0] is the
 *       number of 'A', and hist[1] is the number of 'B', etc.
 * block_size: the buffer size to be used.
 * milliseconds: time it took to complete the file scan
 * total_bytes_read: the amount data in bytes read
 *
 * returns: -1 if there is an error.
 */
int get_histogram(
    FILE *file_ptr, 
    long hist[], 
    int block_size, 
    long *milliseconds, 
    long *total_bytes_read) {
	// TODO
	char buffer[block_size];
	long bytes_read;
	struct timeb time;
	int i;

	do {
		ftime(&time);
		long time_before = time.time*1000 + time.millitm;

		bytes_read = fread(buffer, 1, block_size, file_ptr);
		if (ferror(file_ptr)){
            return -1;
        }
        ftime(&time);
        long time_after = time.time*1000 + time.millitm;

        *milliseconds += time_after - time_before;
        *total_bytes_read += bytes_read;

        for (i = 0; i < sizeof(buffer); i+=sizeof(char)) {
        	int ind = (buffer[i] - 'A') % 26;
        	hist[ind] += 1;
        }


	} while (bytes_read == block_size);

	return 0;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: get_histogram <filename> <blocksize>\n");
		return -1;
	}
	char *filename = argv[1];
	FILE *file_ptr = fopen(filename, "r");

    if (file_ptr == NULL){
        printf("The filename entered does not exist in current directory\n");
        return -1;
    }

	long hist[26] = {[0 ... 25] = 0};
	long milliseconds = 0;
	long filelen = 0;
	long block_size = strtol(argv[2], NULL, 10);
	

	int ret = get_histogram(file_ptr, hist, block_size, &milliseconds, &filelen);
	
	assert(!(ret < 0));

	printf("Computed the histogram in %ld ms.\n", milliseconds);
	for (int i=0; i < 26; i++) {
		// printf("%c : %ld\n", 'A'+i, hist[i]);
	}
    printf("BLOCK SIZE %lu bytes\n", block_size);
    printf("TOTAL BYTES %lu bytes\n", filelen);
    printf("TIME %lu milliseconds\n", milliseconds);
    fclose(file_ptr);
}



// Computed the histogram in 67 ms.
// A : 2031647
// B : 2555943
// C : 2228258
// D : 2752554
// E : 2490406
// F : 3211313
// G : 2228258
// H : 3080239
// I : 2228258
// J : 2424869
// K : 2687017
// L : 3080239
// M : 2621480
// N : 2228258
// O : 2359332
// P : 2359332
// Q : 2949165
// R : 2555943
// S : 2752554
// T : 3145776
// U : 2293795
// V : 2490406
// W : 2490406
// X : 2162721
// Y : 2818091
// Z : 2883628
// BLOCK SIZE 1024 bytes
// TOTAL BYTES 67108864 bytes
// TIME 67 milliseconds