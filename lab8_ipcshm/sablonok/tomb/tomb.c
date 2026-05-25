/* 
  Sablon a "Tömb feltöltése több folyamat által" feladathoz
  
  A TODO sorok nincsenek beírva!

  Ami meg van oldva:
  1. apa folyamat több fiút indít
  2. apa folyamat leállítja minden fiát TERM jelzéssel
 */

#include "../../myinclude.h"

#define K 3  //fiúk száma
#define N 10 //tömb hossza
#define MS 20 //időzítés kritikus szakasz 

typedef struct
{
    int arr[N];
    int index;
}MEMORY;

MEMORY *mem;

int semid;


void fiu_feladata(int i)
{
    struct sembuf fiu_down[] = {{0, -1, 0}, {1, -1, 0}};
    struct sembuf fiu_up = {0, +1, 0};
    printf("start: %d fiú pid=%d, csoport=%d\n", i, getpid(), getpgrp());
    while(1){
        //TODO: a fiúk feladata

        semop(semid, fiu_down, 2);
        mem->arr[mem->index++] = i;
        usleep(10000);
        semop(semid, &fiu_up, 1);
    }
    printf("end: fiú pid=%d\n", getpid());
}

int main(int argc, char **argv)
{
    int status;
    pid_t pid;
    int children = 0; //elindított fiúk száma
    pid_t * pids = NULL ; //fiúk pid számát tároló tömb mutatója
    int exited=0; //kilépett fiúk száma

    setbuf(stdout,NULL);

    printf("apa pid=%d, csoport=%d\n", getpid(), getpgrp());

    //TODO: előkészítés: szemafor + szemafor init + memória + felcsatolás

    int shmid=shmget(IPC_PRIVATE, sizeof(MEMORY), IPC_CREAT | 0666);
    if(shmid < 0)
        syserr("shmget");

    mem = (MEMORY *)shmat(shmid, NULL, 0);
    if((void *)mem == (void *)-1)
        syserr("shmat");

    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    if(semid < 0)
        syserr("semget");

    int init[] = {1, N};
    if(semctl(semid, 0, SETALL, init) < 0)
        syserr("semctl");

    int i;
    for (i = 0; i < K; i++)
    {
        pid = fork();
        if (pid == -1){
            syserr("fork hiba");
        }
        if (pid == 0)
        {
            fiu_feladata(i);
            exit(EXIT_SUCCESS);
        }
        //tömb növelése
        pids = (pid_t*) realloc(pids,(i+1)*sizeof(pid_t));
        if (pids==NULL){
            syserr("pids");
        }
        pids[children++]=pid;
    }

    //TODO: apa feladata
    //apa kód
    if(argc != 2)
        printf("Kell paraméterként egy szám, hogy hányszor fusson!");

    struct sembuf apa_wait_for_full[] = {{0, -1, 0}, {1, 0, 0}};
    struct sembuf apa_up[] = {{0, +1, 0}, {1, +N, 0}};

    for(i = 0; i < atoi(argv[1]); i++){
        semop(semid, apa_wait_for_full, 2);
        for(int j=0; j < N; j++){
            printf("%d ", mem->arr[j]);
        }
        printf("\n");
        mem->index = 0;
        semop(semid, apa_up, 2);
    }


    usleep(300000); //a fiúk induljanak el, ha nem vár, lehet, hogy még nem futnak 

    //fiúk leállítása
    while(children--){
        if (kill(pids[children], SIGTERM)<0){
            syserr("kill");
        }
    }
    int wpid;
    while (exited != K)
    {
        wpid=wait(&status);
        exited++;
        printf("a pid=%d azonosítójú fiú kilépett\n", wpid);
    }

    free(pids);
    if(shmdt((void *)mem) < 0)
        syserr("shmdt");
    
    if(shmctl(shmid, IPC_RMID, 0))
        syserr("shmctl");

    if(semctl(semid, 0, IPC_RMID, 0))
        syserr("semctl");

    exit(EXIT_SUCCESS);
}
