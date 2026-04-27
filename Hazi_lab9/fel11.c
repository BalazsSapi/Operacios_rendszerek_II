#include "myinclude.h"

int main()
{
    int pipeDadWrite[2];
    int pipeChildWrite[2];
    if (pipe(pipeDadWrite) < 0 || pipe(pipeChildWrite) < 0)
    {
        syserr("pipe");
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        syserr("fork");
    }
    if (pid == 0)
    {
        close(pipeDadWrite[1]);
        close(pipeChildWrite[0]);

        double num;
        
        FILE *pipeIn = fdopen(pipeDadWrite[0], "r");
        FILE *pipeOut = fdopen(pipeChildWrite[1], "w");

        if (pipeIn == NULL || pipeOut == NULL)
        {
            syserr("fdopen");
        }
        while (fscanf(pipeIn, "%lf", &num) != EOF)
        {
            if(fprintf(pipeOut, "%lf\n", num*2) < 0)
            {
                syserr("fprinf");
            }
            fflush(pipeOut);
        }
        fclose(pipeIn);
        fclose(pipeOut);

        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipeDadWrite[0]);
        close(pipeChildWrite[1]);

        FILE *pipeIn = fdopen(pipeChildWrite[0],"r");
        FILE *pipeOut = fdopen(pipeDadWrite[1],"w");
        FILE *inFile = fopen("be.txt","r");

        if (pipeIn == NULL || pipeOut == NULL || inFile == NULL)
        {
            syserr("fdopen");
        }

        double num1, num2;

        while(fscanf(inFile, "%lf %lf", &num1, &num2) == 2){
            //printf("%lf %lf\n",num1,num2);
            if(fprintf(pipeOut, "%lf\n", num2) < 0){
                syserr("fprinf");
            }
            fflush(pipeOut);
            fscanf(pipeIn, "%lf", &num2);
            printf("%lf\n", num1/2+num2);
        }

        fclose(pipeOut);
        fclose(pipeIn);

        int status;
        wait(&status);
        
        if(WIFEXITED(status)){
            printf("A fiu kilépési kódja: %d", WEXITSTATUS(status));
        }
        exit(EXIT_SUCCESS);
    }
}