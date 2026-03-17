#include "../myinclude.h"
#include "../parancssor.h"

int main (int argc, char * argv[])
{
    pid_t pid;   //folyamat azonosito
    int status;  //kilepesi allapot tarolasara

    pid = fork();  //uj folyamat
    if ( pid < 0){
        syserr("fork");
    }

    if ( pid == 0){
        execlp("sort", "sort", "-rn", "szam.txt", "-o", "ki.txt", (char*) NULL);
        syserr("execl");
    } else
    {  
        wait (&status);
        if (WIFEXITED(status)) {
            printf("A fiu %d exit koddal lepett ki.\n", WEXITSTATUS(status));
            if (access("ki.txt", F_OK ) == -1) {
                printf("A ki.txt fájl nem jött létre.\n");
            } else {
                printf("A ki.txt fájl sikeresen létre lett hozva.\n");

                int fd = open("legkisebb.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    syserr("open");
                }
                if(dup2(fd, STDOUT_FILENO) == -1) {
                    syserr("dup2");
                }
                int fd2 = open("ki.txt", O_RDONLY);
                if (fd2 == -1) {
                    syserr("open");
                }
                if(dup2(fd2, STDIN_FILENO) == -1) {
                    syserr("dup2");
                }
                execlp("tail", "tail", "-1", (char*) NULL);
            }
    } else if (WIFSIGNALED(status)) {
            printf("A fiu %d jelzessel lepett ki.\n", WTERMSIG(status));
        } else {
            printf("A fiu ismeretlen modon lepett ki.\n");
        }

        
    }
    exit(EXIT_SUCCESS);
}
