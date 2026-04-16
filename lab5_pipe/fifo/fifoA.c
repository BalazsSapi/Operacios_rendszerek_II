#include "myinclude.h"

int main(int argc, char const *argv[])
{
    if (argc != 1) {
        printf("Nincs parancssori paraméter\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen("befifo.txt", "r");
    if (!fp) {
        syserr("fopen");
    }

    // van-e ilyen nevű fájl
    if (access("fifoFile", F_OK)) {  // ha nem létezik
        // létrehozza a névvel rendelkező csövet
        // lásd man 3 mkfifo
        if (mkfifo("fifoFile", 0644) < 0)
            syserr("mkfifo");
    }

    // megnyitja  a fifo-t írásra
    FILE *fifo = fopen("fifoFile", "w");
    if (!fifo) {
        syserr("fopen");
    }
    
    float n;
    while (fscanf(fp, "%f", &n) == 1) {
        fprintf(fifo, "%f\n", n);
    }
    fclose(fp);
    fclose(fifo);
    return 0;
}
