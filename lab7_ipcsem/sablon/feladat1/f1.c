/*
  feladat 1 sablon f1.c
*/

#include "myinclude.h"
#include "f12.h"

int main(int argc, char *argv[])
{
    int semid;     //szemafor tömb
    int fid;       //fájl
    char a = 'a';  //ezt írjuk a fájlba
    int count = N; //számláló

    //az f1 hozza létre és inicializálja  a szemaforokat
    //TODO: ftok hívás paraméterei
    key_t key = ftok(FTOK_FILE, FTOK_ID); 
    
    //TODO:az init a kezdő értékeket tartalmazza
    short init[2] = {1, 0}; //a két szemafor kezdő értéke

    //TODO: a két művelet f1 számára
    struct sembuf upF2 = {1, 1, 0}; //első paraméter: f2 szemafora
    struct sembuf downF1 = {0, -1, 0}; //első paraméter: f1 szemafora

    //TODO: szemafor tömb lekérés, hozza létre ha nem létezik
    if((semid = semget(key, 2, 0660 | IPC_CREAT)) == -1)
        syserr("semget");
    //TODO: szemafor tömb inicializálás
    if ((semctl(semid, 2, SETALL, init)) == -1)
        syserr("semctl");

    //TODO: fájl megnyitása írásra, O_WRONLY | O_TRUNC | O_CREAT | O_APPEND
    if ((fid = open("ab_out.txt", O_WRONLY | O_TRUNC | O_CREAT | O_APPEND, 0660)) == -1)
        syserr("open");

    //FILE *file = fopen("ab_out.txt","a");
    

    //íro ciklus
    while (count--)
    {
        //TODO: kritikus szakaszba lép
        if(semop(semid,&downF1,1) == -1)
            syserr("semop");

        //TODO: ír
        if (write(fid, &a, 1) == -1)
            syserr("write");    
        
        //fprintf(file, "%c\n", a);

        //kilép a kritikus szakaszból
        semop(semid, &upF2, 1);
    }

    //zárja a fájlt
    if (close(fid) == -1)
        syserr("close");

    //fclose(file);

    exit(0);
}