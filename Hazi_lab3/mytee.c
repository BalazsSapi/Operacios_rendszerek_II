#include "getopt.h"
#include "myinclude.h"
#include "stdbool.h"
#include "stddef.h"

#define BUFFSIZE 16

// a fejléc fájlokban definiálva  vannak:
// extern int optopt; // - az utolsó beolvasott opció
// extern int optind; // - az index az argv-ben ahol elmaradt
// extern char * optarg; // - ha az opciónak van argumentuma is, ez arra mutat

int main(int argc, char *argv[]) {
    bool af = false;  // opciók megjegyzéséhez használt jelző változók

    opterr = 0;  // ha ez 0, akkor a getopt() nem ír hibaüzeneteket
                 // amikor meghívjuk
    // csak egy opció van, a: -a
    int ret;
    while ((ret = getopt(argc, argv, "a")) != -1) {
        switch (ret) {
            case 'a':
                af = true;  // a opciót talált
                            // bejelöljük az af kapcsolót
                break;
            case '?':  // ismeretlen opcióra ?-t kap
                fprintf(stderr, "ismeretlen opció -%c\n", optopt);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "optind hiba\n");
                exit(EXIT_FAILURE);
        }
    }

    // beállított opciók

    if (af) {
        printf("a opció beállítva\n");
    }
    // int optind : az első paraméter indexe ami nem opció
    for (int i = optind; i < argc; i++) {
        printf("fennmaradó paraméter: %s\n", argv[i]);
    }
    // printf ("Optind %d argc %d\n", optind, argc);

    if (argc == optind) {
        printf("nincs fájlnév\n");
        exit(1);
    } /*else {
        printf("A fájlnév %s\n", argv[optind]);
    }*/

    int fd[argc - optind];
    if(af == true){
        for(int i=optind; i<argc; i++){
            fd[i-optind] = open(argv[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (fd[i-optind] < 0) {
                syserr("open");
            }
        }
    }
    else {
        for(int i=optind; i<argc; i++){
            fd[i-optind] = open(argv[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd[i-optind] < 0) {
                syserr("open");
            }
        }
    }

    unsigned char buff[BUFFSIZE];
    int n;

    while ((n = read(STDIN_FILENO, buff, BUFFSIZE)) > 0 ){
        write(STDOUT_FILENO, buff, n);
        for (int i=0; i<argc-optind; i++){
            write(fd[i], buff, n);
        }
    }

    for (int i=0; i<argc-optind; i++){
        if (close(fd[i])<0) {
            syserr("close"); //zárjuk a fájlt
        }
    }

    exit(EXIT_SUCCESS);
}
