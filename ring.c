#include "ring.h"

int main(int argc, char * argv[])
{

    knot client_knot;

    memset(&client_knot, 0, sizeof(knot));


    client_knot.self_key = atoi(argv[1]);
    if(client_knot.self_key > 32 || client_knot.self_key < 1)
    {
        printf("Key must be within ring limits (1:32)\n");
        exit(1);
    }

    strcpy(client_knot.self_IP, argv[2]);
    strcpy(client_knot.self_Port, argv[3]);

    printf("KEY: \t %d\nIP: \t %s\nPORT: \t %s\n", client_knot.self_key, client_knot.self_IP, client_knot.self_Port);

    //User Interface
    while(1){

        char input[128] = "", command[12] = "";
        fgets(input, 128, stdin);
        sscanf(input, "%s", command);

        
        // printf("THIS: %s\n", command);

        if(!strcmp("new", command) || !strcmp("n", command))
        {
            printf("\e[1;1H\e[2J");
            New(&client_knot, client_knot.self_key, client_knot.self_IP, client_knot.self_Port);
        }

            

        else if(!strcmp("bentry", command) || !strcmp("b", command))
        {
            short boot;
            char boot_IP[16] = "", boot_Port[6] = "";

            printf("\e[1;1H\e[2J");
            sscanf(input, "%*s %hi %s %s", &boot, boot_IP, boot_Port);
            //printf("BOOT: \t %d\nIP: \t %s\nPORT: \t %s\n", boot, boot_IP, boot_Port);

            Bentry(&client_knot, &boot, boot_IP, boot_Port);
        }
        else if(!strcmp("pentry", command) || !strcmp("p", command))
        {
            short pred;
            char pred_IP[16] = "", pred_Port[6] = "";

            sscanf(input, "%*s %hi %s %s", &pred, pred_IP, pred_Port);
            printf("\e[1;1H\e[2J");
            Pentry(&client_knot, &pred, pred_IP, pred_Port);
        }
        else if(!strcmp("show", command) || !strcmp("s", command))
        {
            
            Show(&client_knot);
            
        }
        else if(!strcmp("exit", command) || !strcmp("e", command))
        {
            printf("EXITING\n");
            exit(0);
        }


    }    

    exit(0);
}






