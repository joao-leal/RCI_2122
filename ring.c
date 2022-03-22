#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>



//Functions Definition
void New(); //Creates a new ring with only 1 knot
void Bentry(int boot, char *IP, char *Port); //Enters a new knot ?
void Pentry(int pred, char *pred_IP, char *pred_Port); // Enters a new knot knowing its predecessor
void Chord(int i, char *i_IP, char *i_Port);
void Echord();
void Show();
void Find(int k);
void Leave();




int main(int argc, char * argv[])
{

    char key[3], i_ip[38], i_Port[5], command[128];


    strcpy(key, argv[1]);
    strcpy(i_ip, argv[2]);
    strcpy(i_Port, argv[3]);

    puts(argv[2]);
    puts(i_ip);

    printf("KEY: %s\nIP: %s : %s\n", key, i_ip, i_Port);

    while(1){

        scanf("%s", command);
        printf("THIS: %s\n", command);

        if(strcmp("new", command) == 0)
            New();

        else if(strcmp("exit", command) == 0)
        {
            printf("EXITING\n");
            exit(0);
        }
            





    }    

    exit(0);
}


void New()
{

    printf("CREATING NEW RING...\n");
}



