#include "myinclude.h"
#include <sys/time.h>
#include <time.h>

int main()
{
    if(access("fifoFile",F_OK)){
        if(mkfifo("fifoFile",0644) < 0){
            syserr("mkfifo");
        }
    }

    FILE *fifoIn = fopen("fifoFile", "r");
    
    float num;

    char buff[25];
    char ms[5];
    struct timeval tv;
    struct tm* ltime;

    while(fscanf(fifoIn,"%f",&num) == 1){
        gettimeofday(&tv, NULL);
        ltime = localtime(&tv.tv_sec);
        strftime(buff, 20, "%Y-%m-%d %T", ltime);
        sprintf(ms, ".%-3.0f", (double)tv.tv_usec / 1000);
        strcat(buff, ms);

        printf("%s %f\n", buff, num);
        //usleep(50000);
    }

    fclose(fifoIn);
    exit(EXIT_SUCCESS);
}