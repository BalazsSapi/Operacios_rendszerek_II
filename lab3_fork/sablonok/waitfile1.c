#include "myinclude.h"
#include <string.h>

void folyamat2(){
    int fd;
    if((fd = open("a.txt", O_WRONLY | O_CREAT, 0644))==-1){
        syserr("open");
    }
    if (write(fd, "Helló", strlen("Helló")) == -1) {
        syserr("write");
    }
    if (close(fd) == -1) {
        syserr("close");
    }
    exit(EXIT_SUCCESS);
}

void folyamat3(){
    while (access("a.txt", F_OK) == -1)
            {
                usleap(100000); // 100 ms
            }
            usleep(20000); // 20 ms
    int fd, n;
    if((fd = open("a.txt", O_RDONLY))==-1) {syserr("open");}
    char buff[BUFSIZ]={0};
    while ((n = read(fd, buff, BUFSIZ)) > 0) {
        write(STDOUT_FILENO, buff, n);
    }
    if (close(fd) == -1) {
        syserr("close");
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (access("a.txt", F_OK) != -1) {
        if(unlink("a.txt") == -1){
            syserr("unlink");
        }
        printf("a.txt fájl törölve\n");
    }
    pid_t pid2, pid3;
    if ((pid2 = fork()) < 0) {
        syserr("f2");
    }
    if(pid2==0){
        //2-es
        folyamat2();
    }
    else{
        if ((pid3 = fork()) < 0) {
            syserr("f3");
        }
        if(pid3==0){
            folyamat3();
        }
        else{
            wait(NULL);
            wait(NULL);
            exit(EXIT_SUCCESS); 
        }
}
