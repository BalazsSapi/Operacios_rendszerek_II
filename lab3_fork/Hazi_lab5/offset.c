/*
 * 3. labor 6. feladat sablon
 *
 * egy paramétere van, a fájl neve
 *
 */

#include "myinclude.h"
#include <string.h>

int main(int argc, char *argv[]) {
    char buff[] = "abc";  // ezt írjuk a fájlba

    // TODO: egy paraméter ellenőrzése, kilépés ha nincs
    if(argc != 2){
        fprintf(stderr, "Használat: %s fájlnév\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd;  // fájl azonosító
    // TODO: fájl megnyitása írásra, csonkolás és új fájl
    if((fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644))==-1){
        syserr("open");
    }
    pid_t pid;  // folyamatazonosító
    // TODO:  fiú indítása a fork() függvénnyel
    if ((pid = fork()) < 0) {
        syserr("fork");
    }

    // fiú kódja
    
    if (pid == 0) {
        // TODO: a fiú beírja a 3 karaktert a fájlba
        // a buff címről
        if (write(fd, buff, strlen(buff)) == -1) {
            syserr("write");
        }
        // zárja a fájlt és kilép
        if (close(fd) < 0) {
            syserr("close");
        }
        exit(EXIT_SUCCESS);  // fiú kilép
    }
    else {
    // apa kódja
    //
    // TODO: apa megvárja  a fiát
    wait(NULL);

    off_t L;
    // TODO: lekérdezzük az L változóba, hol van az offset a
    // fájlban: a SEEK_CUR-hoz képest 0 pozícióba visszük
    // tk. az offset nem fog elmozdulni

    if ((L = lseek(fd, 0, SEEK_CUR)) == -1) {
        syserr("lseek");
    }

    // kiírjuk, hol van az offset
    printf("poz: %ld\n", L);

    // TODO: az apa is beír 3 karaktert
    if (write(fd, buff, strlen(buff)) == -1) {
        syserr("write");
    }

    close(fd);
    exit(EXIT_SUCCESS);
    }
}
