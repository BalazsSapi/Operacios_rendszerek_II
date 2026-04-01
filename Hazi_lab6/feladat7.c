#include "myinclude.h"
#include "parancssor.h"

int main (int argc, char * argv[])
{
    pid_t pid;   //folyamat azonosito
    int status;  //kilepesi allapot tarolasara
    
    pid = fork();  //uj folyamat
    if ( pid < 0){
        syserr("fork");
    }

    if ( pid == 0){
        int fd=open("ki.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            syserr("open");
        }
        dup2(fd, STDOUT_FILENO);
        /*if (parancssor( "egrep ^.{9}[^'].$ /usr/share/dict/words")==-1) {
            syserr("execvp");
        }*/
        execlp("egrep", "egrep", "^.{9}[^'].$", "/usr/share/dict/words", (char *)NULL);
        syserr("execlp");
    } else
    {  
        wait (&status);
        if (WIFEXITED(status)) {
            int fd2 = open("ki.txt", O_RDONLY);
            if (fd2 == -1) {
                syserr("open");
            }
            dup2(fd2, STDIN_FILENO);
            if (access("ki.txt", F_OK ) == -1 || lseek(fd2, 0, SEEK_END) == 0) {
                printf("A ki.txt fájl nem jött létre.\n");
            } else {
                /*if (parancssor("shuf") == -1) {
                    syserr("execvp");
                }*/
                lseek(fd2, 0, SEEK_SET);
                execlp("shuf", "shuf", (char *)NULL);
                syserr("execlp");
            }
    } else if (WIFSIGNALED(status)) {
            printf("A fiu %d jelzessel lepett ki.\n", WTERMSIG(status));
        } else {
            printf("A fiu ismeretlen modon lepett ki.\n");
        }
    }
    exit(EXIT_SUCCESS);
}
