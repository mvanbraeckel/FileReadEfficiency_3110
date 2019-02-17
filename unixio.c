/**
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * 15/02/2019
 * CIS*3110: Operating Systems A2 - System Call vs Function Call: File Reading Efficiency
 *  --> Compares C function calls and Unix system calls in regards to efficiency of reading a file
 */

// =================================== INCLUDES ===================================
#include <fcntl.h> // open
#include <unistd.h> // read
#include <sys/types.h> // read
#include <sys/uio.h> // read
#include <stdio.h> // fopen, fread
#include <stdlib.h>
#include <sys/time.h> // gettimeofday
#include <string.h>
#include <ctype.h>

// ============================== FUNCTION PROTOTYPES =============================
void startTimer();
void stopTimer(char *str);
int isNumber(char *input);

// ================================================================================

struct timeval start, end; // maintain starting and finishing wall time.

int main( int argc, char *argv[] ) {
    // check that proper #of arguments were inputted
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <numBytes> <ioCallType>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // declare variables and convert arguments
    int numBytes = atoi(argv[2]);
    int typeofcalls = atoi(argv[3]);

    // validate arguments: check bytes and type
    if(!isNumber(argv[2]) || numBytes == 0) {
        fprintf(stderr, "Number of bytes must be a positive integer\n");
        fprintf(stderr, "Usage: %s <filename> <numBytes> <ioCallType>\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if(!isNumber(argv[3]) || typeofcalls > 1) {
        fprintf(stderr, "I/O Call Type must be 0 (for C function call) or 1 (for Unix system call)\n");
        fprintf(stderr, "Usage: %s <filename> <numBytes> <ioCallType>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // create proper sized buffer for reading the file
    void *buffer = malloc(numBytes+1);

    // check which I/O call type to use
    if(typeofcalls == 1) {
        // Use unix I/O system calls
        // init print and start timer
        printf("Using Unix I/O systems calls to read a file by %d bytes per fread\n", numBytes);
        startTimer();
        // open (check failure), read until failure, close, calc and display elapsed time
        int fd = open(argv[1], O_RDONLY);
        if(fd == -1) {
            fprintf(stderr, "ERROR: File '%s' does not exist or could not be opened\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        while(read(fd , buffer, numBytes));
        close(fd);
        stopTimer("Unix read");

    } else if(typeofcalls == 0) {
        // Use C standard I/O function calls (remember to use fgetc when numBytes is 1)
        if(numBytes == 1) {
            // init print and start timer
            printf("Using C functions to read a file by %d bytes per fread\n", numBytes);
            startTimer();
            // open (check failure), read until failure, close, calc and display elapsed time
            FILE *fp = fopen(argv[1], "r");
            if(fp == NULL) {
                fprintf(stderr, "ERROR: File '%s' does not exist or could not be opened\n", argv[1]);
                exit(EXIT_FAILURE);
            }
            while(fgetc(fp) != EOF);
            fclose(fp);
            stopTimer("C fgetc");

        } else {
            // init print and start timer
            printf("Using C functions to read a file by %d bytes per fread\n", numBytes);
            startTimer();
            // open (check failure), read until failure, close, calc and display elapsed time
            FILE *fp = fopen(argv[1], "r");
            if(fp == NULL) {
                fprintf(stderr, "ERROR: File '%s' does not exist or could not be opened\n", argv[1]);
                exit(EXIT_FAILURE);
            }
            while(!feof(fp)) fread(buffer, numBytes, 1, fp);
            fclose(fp);
            stopTimer("C fread");
        }

    } else {
        fprintf(stderr, "I/O Call Type must be 0 (for C function call) or 1 (for Unix system call)\n");
        fprintf(stderr, "Usage: %s <filename> <numBytes> <ioCallType>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    free(buffer);
    return 0;
}

/**
 * Memorize the starting time
 */
void startTimer() {
    gettimeofday( &start, NULL );
}

/**
 * Checking the finishing time and computes the elapsed time
 */
void stopTimer(char *str) {
    gettimeofday( &end, NULL );
    printf("%s's elapsed time\t= %ld\n",str, ( end.tv_sec - start.tv_sec ) * 1000000 +
    (end.tv_usec - start.tv_usec));
}

/**
 * Checks if the string is a postive number (or zero)
 * @param char *input -the string to be checked
 * @return 1 if the string only contains digits
 */
int isNumber(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isdigit(input[i])) {
            return 0; //contained a non-digit character
        }
    }
    return 1;
}