#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>



//Functions Definition
void New(); //Creates a new ring with only 1 knot
void Bentry(int boot, char *IP, char *Port);


int main(void)
{

    char command;

    while(1){

        scanf("%c", &command);

        printf("THIS: %c\n", command);

        switch (command)
        {
        case 'e':
            exit(0);
            break;
        
        default:
            break;
        }



    }    

    exit(0);
}




