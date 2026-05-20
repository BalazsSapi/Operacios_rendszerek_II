/*#include "myinclude.h"
#include "randlimit.h"

int main()
{
    int semid;
    pid_t pid1, pid2;
    int N = 6; //ciklusok száma
    int r, i, fd;

    //megoldás 3 szemaforral
    short init[] = {0, 1, 1}; //kezdőértékek, fiúk lépnek először

    //apa szemafor műveletei
    struct sembuf apa_down = {0, -2, 0};
    struct sembuf apa_up[] = {{1, +1, 0}, {2, +1, 0}};
    //első fiú szemafor down művelete
    struct sembuf fiu1_down = {1, -1, 0};
    //második fiú szemafor down művelete
    struct sembuf fiu2_down = {2, -1, 0};
    //mindkét fiú up művelete
    struct sembuf fiuk_up = {0, +1, 0};

    setbuf(stdout,NULL);

    //TODO: itt megoldani
    
    //mindkét fiúban használjuk az 
    //  srand(getpid()) hívást, hogy más random számsort kapjunk

    if((semid = semget(IPC_PRIVATE, 3, 0660 | IPC_CREAT)) < 0)
        syserr("semget");

    if(semctl(semid, 0, SETALL, init) < 0)
        syserr("semctl");

    if((pid1 = fork()) < 0)
        syserr("fork");
    
    if(pid1 == 0){
        // fiú 1

        srand(getpid());

        for(i = 0; i < N; i++){
            r = randlimit(100);

            if(semop(semid, &fiu1_down, 1) < 0)
                syserr("semop");

            fd = open("elso.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
            if(fd < 0)
                syserr("open");
            if(write(fd, &r, sizeof(int)) < 0)
                syserr("write");
            close(fd);

            if(semop(semid, &fiuk_up, 1) < 0)
                syserr("semop");
        }

        exit(EXIT_SUCCESS);
    }
    else{
        if((pid2 = fork()) < 0)
            syserr("fork");
    
        if(pid2 == 0){
            // fiú 2

            srand(getpid());

            for(i = 0; i < N; i++){
                r = randlimit(100);

                if(semop(semid, &fiu2_down, 1) < 0)
                    syserr("semop");

                fd = open("masodik.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
                if(fd < 0)
                    syserr("open");
                if(write(fd, &r, sizeof(int)) < 0)
                    syserr("write");
                close(fd);

                if(semop(semid, &fiuk_up, 1) < 0)
                    syserr("semop");
            }

            exit(EXIT_SUCCESS);
        }
        // apa
        int r1, r2;

        for(i = 0; i < N; i++){
            if(semop(semid, &apa_down, 1) < 0)
                syserr("semop");

            fd = open("elso.txt", O_RDONLY);
            if(fd < 0)
                syserr("open");
            if(read(fd, &r1, sizeof(int)) < 0)
                syserr("read");
            close(fd);

            fd = open("masodik.txt", O_RDONLY);
            if(fd < 0)
                syserr("open");
            if(read(fd, &r2, sizeof(int)) < 0)
                syserr("read");
            close(fd);

            printf("%d\n", abs(r1-r2));

            if(semop(semid, apa_up, 2) < 0)
                syserr("semop");
        }

        wait(NULL);
        wait(NULL);

        if(semctl(semid, 0, IPC_RMID, 0) < 0)
            syserr("semctl");

        exit(EXIT_SUCCESS);
    }
}*/

/*
//megoldás 3 (4 szemaforral)
#include "myinclude.h"
#include "randlimit.h"

int main()
{
    int semid;
    pid_t pid1, pid2;
    int N = 6; //ciklusok száma
    int r, i, fd;

    //megoldás 4 szemaforral
    short init[] = {0, 0, 1, 1}; //kezdőértékek, fiúk lépnek először

    //apa szemafor műveletei
    struct sembuf apa_down[] = {{0, -1, 0}, {1, -1, 0}};
    struct sembuf apa_up[] = {{2, +1, 0}, {3, +1, 0}};
    //első fiú szemafor down művelete
    struct sembuf fiu1_down = {2, -1, 0};
    struct sembuf fiu1_up = {0, +1, 0};
    //második fiú szemafor down művelete
    struct sembuf fiu2_down = {3, -1, 0};
    struct sembuf fiu2_up = {1, +1, 0};
    //mindkét fiú up művelete
    
    

    setbuf(stdout,NULL);

    //TODO: itt megoldani
    
    //mindkét fiúban használjuk az 
    //  srand(getpid()) hívást, hogy más random számsort kapjunk

    if((semid = semget(IPC_PRIVATE, 4, 0660 | IPC_CREAT)) < 0)
        syserr("semget");

    if(semctl(semid, 0, SETALL, init) < 0)
        syserr("semctl");

    if((pid1 = fork()) < 0)
        syserr("fork");
    
    if(pid1 == 0){
        // fiú 1

        srand(getpid());

        for(i = 0; i < N; i++){
            r = randlimit(100);

            if(semop(semid, &fiu1_down, 1) < 0)
                syserr("semop");

            fd = open("elso.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
            if(fd < 0)
                syserr("open");
            if(write(fd, &r, sizeof(int)) < 0)
                syserr("write");
            close(fd);

            if(semop(semid, &fiu1_up, 1) < 0)
                syserr("semop");
        }

        exit(EXIT_SUCCESS);
    }
    else{
        if((pid2 = fork()) < 0)
            syserr("fork");
    
        if(pid2 == 0){
            // fiú 2

            srand(getpid());

            for(i = 0; i < N; i++){
                r = randlimit(100);

                if(semop(semid, &fiu2_down, 1) < 0)
                    syserr("semop");

                fd = open("masodik.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
                if(fd < 0)
                    syserr("open");
                if(write(fd, &r, sizeof(int)) < 0)
                    syserr("write");
                close(fd);

                if(semop(semid, &fiu2_up, 1) < 0)
                    syserr("semop");
            }

            exit(EXIT_SUCCESS);
        }
        // apa
        int r1, r2;

        for(i = 0; i < N; i++){
            if(semop(semid, apa_down, 2) < 0)
                syserr("semop");

            fd = open("elso.txt", O_RDONLY);
            if(fd < 0)
                syserr("open");
            if(read(fd, &r1, sizeof(int)) < 0)
                syserr("read");
            close(fd);

            fd = open("masodik.txt", O_RDONLY);
            if(fd < 0)
                syserr("open");
            if(read(fd, &r2, sizeof(int)) < 0)
                syserr("read");
            close(fd);

            printf("%d\n", abs(r1-r2));

            if(semop(semid, apa_up, 2) < 0)
                syserr("semop");
        }

        wait(NULL);
        wait(NULL);

        if(semctl(semid, 0, IPC_RMID, 0) < 0)
            syserr("semctl");

        exit(EXIT_SUCCESS);
    }
}*/

// megoldás 2 (2 szemaforral, zéróra várással)
#include "myinclude.h"
#include "randlimit.h"

int main()
{
    int semid;
    pid_t pid1, pid2;
    int N = 6; //ciklusok száma
    int r, i, fd;

    // Kezdőértékek: Sem 0 (fiúk számlálója) = 2, Sem 1 (apa engedélye) = 0
    short init[] = {2, 0}; 

    // Fiúk különálló műveletei (nem egy tömbben, hogy ne blokkolják magukat!)
    struct sembuf fiu_down = {0, -1, 0};         // Jelzi, hogy kész a fájl
    struct sembuf fiu_var_egymasra = {0, 0, 0};  // ZÉRÓRA VÁR: megvárja a másik fiút
    struct sembuf fiu_fogyaszt = {1, -1, 0};     // Elvesz egy tokent az apától (blokkol, ha nincs)

    // Apa műveletei
    struct sembuf apa_var_fiukra = {0, 0, 0};    // ZÉRÓRA VÁR: amíg mindkét fiú le nem vonta a magáét
    // Reset: fiúk számlálója vissza 2-re, és 2 token a fiúknak, hogy indulhassanak
    struct sembuf apa_kienged[] = {{0, +2, 0}, {1, +2, 0}};

    setbuf(stdout, NULL);

    if((semid = semget(IPC_PRIVATE, 2, 0660 | IPC_CREAT)) < 0)
        syserr("semget");

    if(semctl(semid, 0, SETALL, init) < 0)
        syserr("semctl");

    if((pid1 = fork()) < 0)
        syserr("fork");
    
    if(pid1 == 0){
        // fiú 1
        srand(getpid());

        for(i = 0; i < N; i++){
            r = randlimit(100);
            
            fd = open("elso.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
            if(fd < 0) syserr("open");
            if(write(fd, &r, sizeof(int)) < 0) syserr("write");
            close(fd);

            // Szinkronizáció lépésenként:
            semop(semid, &fiu_down, 1);         // Sem 0 csökkentése
            semop(semid, &fiu_var_egymasra, 1); // Megvárjuk, míg a Sem 0 eléri a 0-t
            semop(semid, &fiu_fogyaszt, 1);     // Megvárjuk az apát, és elvesszük a tokent
        }
        exit(EXIT_SUCCESS);
    }
    else{
        if((pid2 = fork()) < 0)
            syserr("fork");
    
        if(pid2 == 0){
            // fiú 2
            srand(getpid());

            for(i = 0; i < N; i++){
                r = randlimit(100);

                fd = open("masodik.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
                if(fd < 0) syserr("open");
                if(write(fd, &r, sizeof(int)) < 0) syserr("write");
                close(fd);

                // Szinkronizáció lépésenként:
                semop(semid, &fiu_down, 1);
                semop(semid, &fiu_var_egymasra, 1);
                semop(semid, &fiu_fogyaszt, 1);
            }
            exit(EXIT_SUCCESS);
        }
        else{
            // apa
            int r1, r2;

            for(i = 0; i < N; i++){
                // Megvárjuk, amíg a fiúk számlálója (Sem 0) lecsökken 0-ra
                if(semop(semid, &apa_var_fiukra, 1) < 0)
                    syserr("semop");

                fd = open("elso.txt", O_RDONLY);
                if(fd < 0) syserr("open");
                if(read(fd, &r1, sizeof(int)) < 0) syserr("read");
                close(fd);

                fd = open("masodik.txt", O_RDONLY);
                if(fd < 0) syserr("open");
                if(read(fd, &r2, sizeof(int)) < 0) syserr("read");
                close(fd);

                printf("%d\n", abs(r1-r2));

                // Reset és a fiúk elindítása egyszerre
                if(semop(semid, apa_kienged, 2) < 0)
                    syserr("semop");
            }

            wait(NULL);
            wait(NULL);

            if(semctl(semid, 0, IPC_RMID, 0) < 0)
                syserr("semctl");

            exit(EXIT_SUCCESS);
        }
    }
}