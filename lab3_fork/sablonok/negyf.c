/*
 *  Labor 3, Feladat 1 sablon
 */

#include "myinclude.h"
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]) {
    pid_t pid1, pid2, pid3;      // folyamat azonosítók
    int status1, status2, status3;  // kilépési állapotok átvételéhez
    char buff[256] ={0};
    strcpy(buff,"A");

    // első fiú indítása
    if ((pid1 = fork()) < 0) {
        syserr("f1");
    }
    if (pid1 == 0) {
        // TODO: első fiú kódja
        printf("B folyamat\n");
        while(access("ki.txt", F_OK ) == -1){
            usleep(200000);
        }
        int fd;
        if((fd=open("ki.txt", O_RDONLY))==-1) syserr("open");
        int n = read(fd, buff, strlen(buff));
        write(STDOUT_FILENO,buff, n);

        close(fd);
        exit(EXIT_SUCCESS);  
    }else{
        if((pid2 = fork()) < 0) {
            syserr("f2");
        }
    if(pid2==0){
        printf("C folyamat\n");
        strcat(buff," C");
        
        if ((pid3 = fork()) < 0) {
            syserr("f3");
        }
        if(pid3==0){
            printf("D folyamat\n");
            strcat(buff," D");
            int fd;
            if((fd = open("ki.txt", O_WRONLY | O_CREAT |O_TRUNC, 0644))==-1) syserr("open");
            if(write(fd, buff,strlen(buff))==-1)syserr("write");
            close(fd);
            exit(EXIT_SUCCESS); 
        }
        wait(&status3);
        exit(EXIT_SUCCESS);
    }
    else{
        printf("A folyamat\n");
        wait(&status1);
        wait(&status2);
        wait(&status3);
     
    }
}
}
