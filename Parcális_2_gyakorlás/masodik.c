#include <stdio.h>
#include "myinclude.h"

int main(){

    int shmid = shmget(ftok("./",1), 2*sizeof(int), IPC_CREAT | 0666 | IPC_EXCL);
    if(shmid < 0)
        syserr("shmget");

    int semid = semget(ftok("./",1), 3, IPC_CREAT | 0666 | IPC_EXCL);
    if(semid < 0)
        syserr("semget");

    struct sembuf harmadik_down = {2, -2, 0};
    struct sembuf harmadik_up[] = {{0, +1, 0}, {1, +1, 0}};
    
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