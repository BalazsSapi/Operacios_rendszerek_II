#include <stdlib.h>
#include "myinclude.h"
#include <limits.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Használat: %s <be fájl> <ki fájl>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pfd[2];
    if (pipe(pfd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        double num, sum = 0, largest, smallest;
        int n, first = 1;
        if(close(pfd[1])==-1)
        {
            syserr("close");
        }
        //FILE *pipeFileIn = fdopen(pfd[0], "r");
        freopen(argv[2], "w", stdout);
        while ((n=read(pfd[0], (void*) &num, sizeof(double)))>0){
            //... feldolgozás
            //itt egy olvasáskor az n-nek sizeof(double) értéknek kell lennie
            //ezt lehet ellenőrizni !!!
            if(first){
                largest = smallest = num;
                first = 0;
            }
            else{
                if(num>largest){
                    largest  = num;
                }
                if(num<smallest){
                    smallest = num;
                }
            }
            sum+=num;
        }
        if (n<0){
            syserr("read");
        }

        printf("Összeg: %.2lf\n", sum);
        printf("Legnagyobb: %.2lf\n", largest);
        printf("Legkisebb: %.2lf\n", smallest);

        fclose(stdout);
        close(pfd[0]);
        //fclose(pipeFileIn);
        exit(EXIT_SUCCESS);
    }
    else // parent process
    {
        double num;
        if(close(pfd[0])==-1)
        {
            syserr("close");
        }
        if(!freopen(argv[1], "r", stdin))
        {
            syserr("freopen");
        }
        while (scanf("%lf", &num) == 1)
        {
            if (write(pfd[1], (void*) &num, sizeof(double)) != sizeof(double))
            {
                syserr("write");
            }
        }
        fclose(stdin);
        close(pfd[1]);
        //fclose(pipeFileOut);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}