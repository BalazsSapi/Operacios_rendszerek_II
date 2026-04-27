#include "myinclude.h"
#include <math.h>

int main()
{
    int pipeA[2],pipeB[2];

    if (pipe(pipeA) || pipe(pipeB)){
        syserr("pipe");
    }

    pid_t pid1,pid2;

    if ((pid1=fork()) < 0){
        syserr("fork");
    }

    if(pid1==0)
    {
        close(pipeA[0]);
        close(pipeB[1]);
        close(pipeB[0]);

        FILE *readFile = fopen("a.txt","r");
        FILE *pipeOut = fdopen(pipeA[1],"w");

        if(!readFile || !pipeOut){
            syserr("fopen/fdopen");
        }

        float num;

        while(fscanf(readFile, "%f", &num) == 1){
            if(fprintf(pipeOut, "%f\n", num) < 0){
                syserr("fprintf");
            }
            fflush(pipeOut);
        }

        fclose(pipeOut);
        fclose(readFile);
        exit(EXIT_SUCCESS);
    }
    else
    {
        if((pid2=fork()) < 0){
            syserr("fork");
        }
        if(pid2 == 0){
            close(pipeB[0]);
            close(pipeA[1]);
            close(pipeA[0]);

            FILE *readFile = fopen("b.txt","r");
            FILE *pipeOut = fdopen(pipeB[1],"w");

            if(!readFile || !pipeOut){
                syserr("fopen/fdopen");
            }

            float num;

            while(fscanf(readFile, "%f", &num) == 1){
                if(fprintf(pipeOut, "%f\n", num) < 0){
                    syserr("fprintf");
                }
                fflush(pipeOut);
            }
            
            fclose(pipeOut);
            fclose(readFile);
            exit(EXIT_SUCCESS);
        }
        else
        {
            close(pipeA[1]);
            close(pipeB[1]); 

            FILE *pipeInA = fdopen(pipeA[0], "r");
            FILE *pipeInB = fdopen(pipeB[0], "r");

            if(!pipeInA || !pipeInB){
                syserr("fdopen");
            }

            if(access("fifoFile",F_OK)){
                if(mkfifo("fifoFile",0644) < 0){
                    syserr("mkfifo");
                }
            }

            FILE *fifoOut = fopen("fifoFile", "w");

            float num1,num2;

            while(fscanf(pipeInA,"%f",&num1)==1 && fscanf(pipeInB,"%f",&num2)==1){
                if(fabs(num1-num2) > 2){
                    fprintf(fifoOut,"%f\n",num1-num2);
                }
                //printf("%f %f\n",num1,num2);
                usleep(50000);
            }

            fclose(pipeInA);
            fclose(pipeInB);
            wait(NULL);

            fclose(fifoOut);
            exit(EXIT_SUCCESS);
        }
        
    }
}