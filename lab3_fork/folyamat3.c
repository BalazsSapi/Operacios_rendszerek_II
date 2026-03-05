#include "myinclude.h"

int main() {
    setbuf(stdout, NULL);
    pid_t pid1;
    int x = 1;

    if ((pid1 = fork()) < 0) {
        syserr("fork1");
    }

    if (pid1==0){
        // 1. fiú kódja
        printf("1. fiú: x=%d, pid=%d\n", x, getpid());
        exit(EXIT_SUCCESS);
    }
    else{
        pid_t pid2;
        x=2;
        if ((pid2 = fork()) < 0) {
            syserr("fork2");
        }
        if (pid2==0)
        {
            // 2. fiú kódja
            printf("2. fiú: x=%d, pid=%d\n", x, getpid());
            exit(EXIT_SUCCESS);
        }
        else{
            // apa kódja
            printf("apa: x=%d, pid=%d\n", x, getpid());
            wait(NULL);
            wait(NULL);
            exit(EXIT_SUCCESS);
        }
    }
    
    return 0;
}
