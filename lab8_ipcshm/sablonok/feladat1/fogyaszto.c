//olvasó folyamat

#include "myinclude.h"
#include "shm.h"

//két szemafort használunk
//kezdőérték s0=1, s1=0
//az olvasó folyamat down: -1->s1
//                   up:   +1->s0

int main( int argc, char * argv[])
{
    int semid, shmid;
    char * mem;    //ide csatoljuk

    //TODO: szemafor műveletek definiálása
    struct sembuf olv_down={1, -1, 0};   //s1 down
    struct sembuf olv_up={0, +1, 0};     //s0 up
                  
    //TODO: szemafor tömb lekérése
    semid=semget(ftok("./", 1), 2, 0);
    if(semid==-1){
        sleep(10);
        semid=semget(ftok("./", 1), 2, 0);
        if(semid==-1)
            syserr("semget");
    }
        

    //TODO: osztott memória id lekérése

    shmid=shmget(ftok("./", 1), 1024*sizeof(char), 0);
    if(shmid==-1){
        sleep(10);
        shmid=shmget(ftok("./", 1), 1024*sizeof(char), 0);
        if(shmid==-1)
            syserr("shmget");
    }

    //TODO: osztott memória felcsatolása

    mem=(char*)shmat(shmid, NULL, 0);
    if ((void *)mem == (void *)-1) 
        syserr("shmat");

    while(1) {

    //TODO:
    // 1. down művelet
    semop(semid, &olv_down, 1);   //s1 down
    // 2. 1 sor olvasása és kiírása: mem->stdout 
    printf("%s",mem);
    // 3. up művelet
    semop(semid, &olv_up, 1);     //s0 up
    //ha  a sor "exit" akkor break
    if(strcmp(mem, "exit\n") == 0 || strcmp(mem, "") == 0)
        break;
    }    

    //TODO: osztott memória lecsatolás
    if(shmdt((void *)mem) == -1)
        syserr("shmdt");

    //TODO: szemafor tömb és osztott memória id törlése
    if(semctl(semid,0,IPC_RMID)<0) syserr("ctl1"); 
    if(shmctl(shmid,IPC_RMID,0)<0) syserr("ctl2"); 



    exit(EXIT_SUCCESS);
}