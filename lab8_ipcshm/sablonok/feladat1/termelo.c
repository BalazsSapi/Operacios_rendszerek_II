// az író folyamat

#include "myinclude.h"
#include "shm.h"

//két szemafort használunk
//kezdőérték s0=1, s1=0
//az író folyamat down: -1->s0
//                up:   +1->s1

int main( int argc, char * argv[])
{
    int semid, shmid;
    char * mem;    //ide csatoljuk

    //TODO: szemafor műveletek definiálása

    struct sembuf iro_down={0, -1, 0};   //s0 down
    struct sembuf iro_up={1, +1, 0};     //s1 up
                  
    //TODO: szemafor tömb létrehozás

    semid=semget(ftok("./", 1), 2, IPC_CREAT | 0666);
    if(semid==-1)
        syserr("semget");

    //TODO: szemafor inicializálás

    short init[2]={1, 0};
    semctl(semid, 0, SETALL, init);   //s0=1
    if(semid==-1)
        syserr("semget");

    //TODO: osztott memória id lekérése

    shmid=shmget(ftok("./", 1), 1024*sizeof(char), IPC_CREAT | 0666);
    if(shmid==-1)
        syserr("shmget");

    //TODO: osztott memória felcsatolása

    mem=(char*)shmat(shmid, NULL, 0);
    if ((void *)mem == (void *)-1) 
        syserr("shmat");

    int n;
    
    while(1){

        //TODO:
        // 1. down művelet
        semop(semid, &iro_down, 1);   //s0 down
        // 2. 1 sor olvasása stdin->mem, max 1024 byte!
        if((n = read(STDIN_FILENO, mem, 1024)) < 0)
            syserr("read");

        mem[n] = '\0';  //string lezárása
        // 3. up művelet
        semop(semid, &iro_up, 1);     //s1 up
        //ha  a sor "exit" akkor break
        if(strcmp(mem, "exit\n") == 0 || n == 0)
            break;
    }
    //osztott memória lecsatolás
    if(shmdt((void *)mem) == -1)
        syserr("shmdt");

    exit(EXIT_SUCCESS);
}