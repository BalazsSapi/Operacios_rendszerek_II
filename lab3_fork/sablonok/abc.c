/*
 *  Labor 3, Feladat 1 sablon
 */

#include "myinclude.h"
#define NCHAR 100000

// n darab c karaktert ír a name nevű fájlba
int makefile(char *name, char c, int n) {
    int fd;
    // TODO: nyissuk meg írásra, új fájl, csonkolással
    fd = open(name,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd==-1){
        syserr("File hiba.");
    }

    // TODO: for ciklus n-szer, minden ciklus egyszer
    // kiírja a c karaktert a fájlba
    for(int i =0; i < n; i++){
        if(write(fd,&c,1)!=1){ syserr("hiba az irasban!");};
    }
    // fájl zárás
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;      // folyamat azonosítók
    int status1, status2;  // kilépési állapotok átvételéhez

    // első fiú indítása
    if ((pid1 = fork()) < 0) {
        syserr("f1");
    }
    if (pid1 == 0) {
        // TODO: első fiú kódja
        printf("Fiu 1\n");
        makefile("a.txt", 'a', NCHAR);
        exit(EXIT_SUCCESS);  // a fiú itt mindenképp kilép, így
                                 // nem futhat rá a következő kódokra
    }else{
        if((pid2 = fork()) < 0) {
        syserr("f2");
    }
    if(pid2==0){
        printf("fiu 2\n");

        makefile("b.txt", 'b', NCHAR);
        exit(EXIT_SUCCESS);
    }
    else{
        printf("apa\n");
        //apa megvarja mindke3t fiat

        makefile("c.txt", 'c', NCHAR);
        wait(&status1);
        wait(&status2);
     
    }
}
}
