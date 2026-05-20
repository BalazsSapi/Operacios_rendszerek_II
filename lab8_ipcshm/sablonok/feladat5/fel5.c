#include "myinclude.h"

enum { semA,
       semB,
       semC };

#define N 10

void A_feladat(int semid, int* mem) {
    struct sembuf downA = {semA, -2, 0};
    struct sembuf upA[]   = {{semB, +1, 0},
                             {semC, +1, 0}};
    bool display = false;
    for (int i = 0; i < N; i++) {
        if (semop(semid, &downA, 1) == -1)
            syserr("semop");
        if(display == false)
            display = true;
        else{
            printf("B: %d C: %d\n", *mem, *(mem + 1));
        }
        *mem = i;
        *(mem + 1) = i;
        if (semop(semid, upA, 2) == -1)
            syserr("semop");
    }
    if(shmdt(mem) == -1)
        syserr("shmdt");
}

void BC_feladat(int semid, int* mem, int ix, short sem) {
    struct sembuf down = {sem, -1, 0};
    struct sembuf up = {semA, +1, 0};
    for (int i = 0; i < N; i++) {
        if (semop(semid, &down, 1) == -1)
            syserr("semop");
        mem[ix] *= 2;
        if (semop(semid, &up, 1) == -1)
            syserr("semop");
    }
    if(shmdt(mem) == -1)
        syserr("shmdt");
}

int main(int argc, char const* argv[]) {
    int semid, shmid;

    if ((semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660)) == -1)
        syserr("semget");
    short init[3] = {2, 0, 0};
    if (semctl(semid, 0, SETALL, init) == -1)
        syserr("semctl");
    if ((shmid = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0660)) == -1)
        syserr("shmget");

    int* mem = (int*)shmat(shmid, NULL, 0);
    if (mem == (void*)-1)
        syserr("shmat");

    pid_t pidB, pidC;

    if ((pidB = fork()) == -1)
        syserr("fork");
    if (pidB == 0) {
        // B
        BC_feladat(semid, mem, 0, semB);
        exit(EXIT_SUCCESS);
    }
    if ((pidC = fork()) == -1)
        syserr("fork");
    if (pidC == 0) {
        // C
        BC_feladat(semid, mem, 1, semC);
        exit(EXIT_SUCCESS);
    }
    // A
    A_feladat(semid, mem);

    wait(NULL);
    wait(NULL);
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        syserr("shmctl");
    if (semctl(semid, 0, IPC_RMID) == -1)
        syserr("semctl");
    return 0;
}