/*
 * Sablon: Labor 3, feladat 7.
 *
 * futtatás méréshez:
 *
 * ./kiazelso 100000
 *
 */

#include "myinclude.h"

int main(int argc, char *argv[]) {
    // ezt a két változót írjuk a fájlba
    char one[] = "1\n";
    char zero[] = "0\n";

    setbuf(stdout, NULL);

    size_t ciklus;
    // TODO: ha nincs parancssori paraméter, ciklus=1000,
    // ha van, akkor atoll() függvénnyel átalakítjuk
    // és beírjuk a ciklus-ba
    // ha 0-t kapunk hiba és kilépünk

        if(argc<2){
            ciklus=1000;
        }
        else if(argc==2){
            ciklus=atoll(argv[1]);
            if(ciklus==0){
                fprintf(stderr, "Hiba: 0 érték nem megengedett\n");
                exit(EXIT_FAILURE);
            }
        }
        else{
            fprintf(stderr, "Hiba: túl sok paraméter\n");
            exit(EXIT_FAILURE);
        }

    int fd;
    // TODO: megnyitjuk ki.txt fájlt írásra, csonkolás, új fájl

    if((fd = open("ki.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
        syserr("open");
    }
    // elvégezzük többször a kiírásokat
    for (size_t i = 0; i < ciklus; i++) {
        pid_t pid;
        // TODO: új folyamat, pid szám a pid változóba
        // ellenőrzés, hogy sikerült

        if((pid = fork()) < 0){
            syserr("fork");
        }

        // fiú:
        if (pid == 0) {
            // TODO: beírja a zero változó tartalmát a fájlba, 2 karaktert

            if (write(fd, zero, 2) == -1) {
                syserr("write");
            }
            exit(EXIT_SUCCESS);
        } else {  // apa:

            // TODO: beírja a one változó tartalmát a fájlba, 2 karaktert
            if (write(fd, one, 2) == -1) {
                syserr("write");
            }

            // TODO: apa megvárja a fiát
            wait(NULL);
        }
    }
    exit(EXIT_SUCCESS);
}
