/*
  feladat 1 sablon f2.c
*/
#include "myinclude.h"
#include "f12.h"

int main( int argc, char * argv[])
{
    int semid; //szemafor tömb 

    //TODO:a két szemafor művelet f2 számára
    struct sembuf downF2 = {1, -1, 0}; //első paraméter: f2 szemafora
    struct sembuf upF1 = {0, 1, 0}; //első paraméter: f1 szemafora
    
    //TODO: a szemafor tömb lekérése, adjon hibát ha nem létezik
    //az f1 által létrehozva
    key_t key = ftok(FTOK_FILE, FTOK_ID);
    if((semid = semget(key, 2, 0660)) == -1)
        syserr("semget");

    //TODO: fájl nyitása írásra, adjon hibát ha nem létezik
    //ne csonkolja
    int fid;  //fájl
    if ((fid = open("ab_out.txt", O_WRONLY | O_APPEND)) == -1)
        syserr("open");

    //íro ciklus
    int count = N; //számláló
    char c = 'b';  //ezt írjuk a fájlba
    while (count--)
    {
        //TODO: kritikus szakaszba lép
        if(semop(semid,&downF2,1) == -1)
            syserr("semop");

        //TODO: ír
        if (write(fid, &c, 1) == -1)
            syserr("write");

        //TODO:kilép a kritikus szakaszból
        if(semop(semid, &upF1, 1) == -1)
            syserr("semop");
    }

    //TODO: törli a szemafor tömböt
    //ez a folyamat használja utoljára
    if (semctl(semid, 0, IPC_RMID) == -1)
        syserr("semctl");
    
    //zárja a fájlt
    close(fid);
    exit(EXIT_SUCCESS);

} 