#include <stdio.h>
#include "myinclude.h"

int main(){

    pid_t pid;
    int p[2];

    int shmid = shmget(ftok("./",1), 2*sizeof(int), IPC_CREAT | 0666 | IPC_EXCL);
    if(shmid < 0)
        syserr("shmget");

    int semid = semget(ftok("./",1), 3, IPC_CREAT | 0666 | IPC_EXCL);
    if(semid < 0)
        syserr("semget");
        
    int init[] = {1, 1, 0};
    if(semctl(semid, 0, SETALL, init) < 0)
        syserr("semctl");

    if(pipe(p) < 0)
        syserr("pipe");

    pid = fork();
    if(pid < 0)
        syserr("fork");
    
    if(pid > 0){
        // 2. folyamat
        close(p[1]);

        struct sembuf masodik_down = {1, -1, 0};
        struct sembuf masodik_up = {2, +1, 0};
        
        int *m = (int *)shmat(shmid, NULL, 0);
        if((void *)m == (void *)-1)
            syserr("shmat");
        int *m2 = m + 1;

        FILE *pin = fdopen(p[0], "r");
        if(!pin)
            syserr("fdopen");

        int num;
        
        while(fscanf(pin, "%d", &num) == 1){
            semop(semid, &masodik_down, 1);
            *m2 = num;
            semop(semid, &masodik_up, 1);
        }

        semop(semid, &masodik_down, 1);
        *m2 = INT_MIN;
        semop(semid, &masodik_up, 1);

        fclose(pin);

        semdt((void *)m);

        exit(EXIT_SUCCESS);
    }
    else{
        // 1. folyamat
        close(p[0]);

        struct sembuf elso_down = {0, -1, 0};
        struct sembuf elso_up = {2, +1, 0};
        
        int *m = (int *)shmat(shmid, NULL, 0);
        if((void *)m == (void *)-1)
            syserr("shmat");

        FILE *file = fopen("adat.txt", "r"), *pout = fdopen(p[1], "w");
        if(!file || !pout)
            syserr("fopen/fdopen");

        int num;
        float sum = 0;
        bool first = true;

        FILE *out = fopen("adat1.txt", "a");
        if(!out)
            syserr("fopen");

        while(fscanf(file, "%d", &num) == 1){
            fprintf(pout, "%d\n", num);
            semop(semid, &elso_down, 1);
            if(first == false){
                sum += ((float)((*m))/2);
                fprintf(out, "%f\n", (float)((*m))/2);
            }
            semop(semid, &elso_up, 1);
        }

        semop(semid, &elso_down, 1);
        sum += ((float)((*m))/2);
        fprintf(out, "%f\n", (float)((*m))/2);
        semop(semid, &elso_up, 1);

        fclose(pout);
        shmdt((void *)m);
        shmctl(shmid, IPC_RMID, 0);
        semctl(semid, 0, IPC_RMID);

        wait(NULL);
        exit(EXIT_SUCCESS);
    }

}