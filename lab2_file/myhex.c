/*
 * fájl nyomtatása hex formátumban
 */

#include "myinclude.h"
#include <ctype.h>

#define BUFFSIZE 16

int main(int argc, char * argv[])
{

    unsigned char buff[BUFFSIZE]; //puffer, egy ciklusban 8 karaktert olvasunk
    int fd;             //fájl azonosító
    int n;              //a read() visszatérített értékének tárolása
    int k;              //puffer index
    int count =0 ;

    //ha nincs egy argumentum
    if (argc<2) {
        fprintf(stderr,"használat: %s fájlnév\n", argv[0]); 
        exit(EXIT_FAILURE);
    }

    //TODO : nyissuk meg csak olvasásra a fájlt
    //megkapjuk az fd-t, 
    fd = open(argv[1], O_RDONLY);
    
    
    //TODO: teszteljük, hogy nincs hiba, ha van kilépünk

    if (fd < 0){
        syserr("open");
    }
    //TODO: olvassunk, amíg a read() által visszatérített érték > 0
    while ( (n = read(fd, buff, BUFFSIZE)) > 0 ){

        //kiírjuk a sor első karakterének sorszámát
        printf("%06x ",count);

        //TODO: kiírjuk a hexa kódokat, egy k indexel járjuk végig a puffert
        // ezt a printf-et használjuk:    printf ("%02x ", buff[k] & 0xff);
        // a %x 16 bites előjel nélküli egészet vár, 
        // ha a karakter felső bitje 1-es, negatív számokat fog kiírni

        for (k = 0; k < n; k++){
            printf ("%02x ", buff[k]);
        }

        //TODO: kiírjuk a nyomtatható karaktereket
        //    ha: isprint(buff[k])) igaz kiírjuk, ha nem egy . karaktert írunk
        // lásd:  man isprint

        for (k = 0; k < n; k++){
            if(isprint(buff[k])){
                printf("%c", buff[k]);
            } else {
                printf(".");
            }
        }

        printf("\n");
        count+=n; //következő sor
    }
    printf("%06x\n",count);

    if (close(fd)<0) 
        syserr("close"); //zárjuk a fájlt

    exit(EXIT_SUCCESS);

}

