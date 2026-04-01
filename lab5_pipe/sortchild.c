/*
 *
 * az apa folyamat egy nagy szöveges fájlt akár megjeleníteni
 * átírja csővezetéken a fiának, amelyben a less nevű
 * listázó programot futtatja
 *
 * tesztelés:s
 *    gcc -Wall useless.c -o useless
 *    ./useless 1.txt
 */

#include "myinclude.h"

/* a puffer hossza */
#define BUFF 4096

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc != 2) {
        printf("használat: %s fájlnév\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    // csővezeték létrehozás
    int pfd[2];
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    if ((pid = fork()) < 0) {
        syserr("fork");
    }

    // apa folyamat, ez ír a csővezetékbe, például az 1.txt nevű
    // nagy méretű állományt másolja
    if (pid > 0) {
        // lezárja a cső olvasható végét
        if (close(pfd[0]) < 0) {
            syserr("close");
        }
        FILE *in;
        // bemeneti fájl
        if ((in = fopen(argv[1], "r")) == NULL) {
            syserr("open");
        }

        // átírja a fájlt a csővezetékbe
        double num;
        FILE *pipeIn = fdopen(pfd[1], "w");
        if(pipeIn == NULL) {
            syserr("fdopen");
        }
        while (fscanf(in, "%lf", &num) == 1) {
            if (fprintf(pipeIn, "%lf\n", num) < 0) {
                syserr("write");
            }
        }
        fclose(in);

        // a cső lezárása fontos!
        close(pfd[1]);
        // megvárja, hogy fia is befejezze
        int status;
        if (wait(&status) < 0)
            syserr("wait");

        exit(EXIT_SUCCESS);
    }
    /* fiú folyamat kódja */
    else {
        close(pfd[1]);  // ez a folyamat olvasni fog, az írható véget lezárja

        // hozzárendelem a pipe olvasható véget az stdin-hez
        /*if (dup2(pfd[0], STDIN_FILENO) < 0) {
            syserr("dup2");
        }*/
        FILE *pipeOut = fdopen(pfd[0], "r");
        double num;
        while (fscanf(pipeOut, "%lf", &num) == 1) {
            printf("%lf\n", num);
        }
        //close(pfd[0]); /* többet nincs rá szükség */

        // elindítom a sort programot, ez a standard bemenetről olvassa
        // a szövegét
        //execlp("sort", "sort", "-n", (char *)NULL);

        /* ide már nem érhetek el */
        /* a sort ki fog lépni, ha lezárjuk a pipe másik végét */
        //syserr("execlp");
        exit(EXIT_SUCCESS);
    }
}
