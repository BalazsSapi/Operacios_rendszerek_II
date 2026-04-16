#include "myinclude.h"

int main(int argc, char const *argv[])
{
    if (argc != 1) {
        printf("Nincs parancssori paraméter\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen("kififo.txt", "w");
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

    // megnyitja  a fifo-t olvasásra
    FILE *fifo = fopen("fifoFile", "r");
    if (!fifo) {
        syserr("fopen");
    }
    
    float n, m;
    while (fscanf(fifo, "%f %f", &n, &m) == 2) {
        fprintf(fp, "%f\n", n+m);
    }
    fclose(fifo);
    fclose(fp);
    return 0;
}