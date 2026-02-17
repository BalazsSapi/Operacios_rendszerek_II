#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char ** environ;

int main( int argc, char * argv[] ) {
    if (argc != 3) {
        printf("Pontosan két sztringet kell megadni!\n");
        exit(-1);
    }

    if (strcmp(argv[1],argv[2]) != 0) {
        printf("Nem egyformák!\n");
        exit(-2);
    }
    else {
        printf("Egyformák!\n");
    }

    exit(EXIT_SUCCESS);
}
