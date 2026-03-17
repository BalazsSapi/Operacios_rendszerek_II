/* 
 * sablon az 1. feladathoz
 *
 */

#include "../myinclude.h"
#include "../parancssor.h"

int main (int argc, char * argv[])
{
    pid_t pid;   //folyamat azonosito
    int status;  //kilepesi allapot tarolasara

    pid = fork();  //uj folyamat
    if ( pid < 0){
        syserr("fork");
    }

    if ( pid == 0){

        //TODO: ide beirni az exec hivasokat
        //egyszerre egyet, kiprobalni es utana
        //magyarazatba tenni
     
        //execlp("sort","sort", "-rn", "szam.txt", (char*) NULL); 
    
        char parancs[]="sort -nr szam.txt";
        if(parancssor(parancs)==EXIT_FAILURE) syserr("execvp");
        //ide nem ter vissza a fiu ha helyes

        syserr("execl");
    } else
    {  
        //apa folyamat megvarja a fiut 
        wait (&status);
        
        //TODO: az apa irja ki, hogy milyen exit koddal 
        //vagy jelzessel lepett ki a fiu
        if (WIFEXITED(status)) {
            printf("A fiu %d exit koddal lepett ki.\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("A fiu %d jelzessel lepett ki.\n", WTERMSIG(status));
        } else {
            printf("A fiu ismeretlen modon lepett ki.\n");
        }
    }
    exit(EXIT_SUCCESS);
}
