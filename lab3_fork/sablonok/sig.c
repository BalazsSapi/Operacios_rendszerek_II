/*
 *
 * Labor 3 , feladat 4.
 *
 */

#include "myinclude.h"

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);  // pufferelés letiltva

    pid_t pid;             // folyamat azonosító
    if ((pid = fork()) < 0) {  // fiú indítása
        syserr("fork");
    }

    // fiú folyamat kódja
    if (pid == 0) {
        while (1) {  // végtelen ciklus
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    } else {
        int status;  // állapot lekérdezéséhez

     
        //egyben van a sig1 feladat

        //TODO: addig tart a while, amíg a waitpid() visszaadja azt
        //hogy a folyamat kilépett
        //a status a kilépési adatokat tartalmazza
        //a waitpid()-et WNOHANG opcióval hívjuk, hogy ne blokkoljon
        int cnt=0;
        while (  waitpid(pid, &status,WNOHANG) ==0 ) {
            printf("tick %d\n",cnt);
            usleep(300000);
            //TODO:
            //ha  a számláló eléri az 5-öt, utána küldjünk TERM jelzést
            //várjuk meg a fiút és lépjünk ki a while-ból break-el
            //a status a kilépési adatokat tartalmazza
            if(++cnt==5){
                if(kill(pid,SIGTERM)==-1) {
                    perror("kill");
                };
            };
        }
        wait(&status);        
        if(WIFEXITED(status)){
            printf("exit: %d\n", WEXITSTATUS(status));
        }
        // ha jelzéssel lépett ki kiírjuk a jelzés azonosítóját
        if (WIFSIGNALED(status)) {
            printf("jelzéssel állt meg a fiú, azonosító: %d\n", WTERMSIG(status));
        }
    }

    printf("apa kilép\n");
    exit(EXIT_SUCCESS);
}
