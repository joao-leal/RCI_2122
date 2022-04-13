#include "ring.h"

int main(int argc, char * argv[])
{

    knot client_knot;
    int max_fd = 0;
    fd_set read_fds;
    enum {idle, busy} state;

    //Initializes knot structure
    memset(&client_knot, 0, sizeof(knot));

    //Prevents SIGPIPE Signal
    struct sigaction act;
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    sigaction(SIGPIPE,&act,NULL);


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
    state = idle;
    while(1){

        char input[MAX_MESSAGE_LENGTH] = "", command[COMMAND_LENGTH] = "";
        int counter = 0;

        printf("LOOP\n");
        
        FD_ZERO(&read_fds);

        switch(state)
        {
            case idle:

            //Adds the file descriptors for TCP & UDP connections and for the STDIN
                FD_SET(client_knot.fd_listen, &read_fds);
                FD_SET(client_knot.fd_UDP, &read_fds);
                max_fd = max(client_knot.fd_listen, client_knot.fd_UDP);
                FD_SET(STDIN_FILENO, &read_fds);
                max_fd = max(max_fd, STDIN_FILENO);
                break;

            case busy:
                break;

        }

        counter = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if(counter <= 0)
            exit(1);
        
        for(;counter;--counter)
        {
            switch(state)
            {
                case idle:

                    /************************************/
                    /*              CLIENT              */
                    /************************************/

                    if(FD_ISSET(STDIN_FILENO, &read_fds))
                    {
                        fgets(input, MAX_MESSAGE_LENGTH, stdin);
                        sscanf(input, "%s", command);
                    
                        // printf("THIS: %s\n", command);

                        if(!strcmp("new", command) || !strcmp("n", command))
                        {
                            printf("\e[1;1H\e[2J");
                            New(&client_knot);
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

                    /************************************/
                    /*              SERVER              */
                    /************************************/

                    //Incoming TCP request
                    if(FD_ISSET(client_knot.fd_listen, &read_fds))
                    {
                        if(client_knot.pred_key == 0) //Knot is not in ring
                        {
                            client_knot.fd_aux = acceptTCP(&client_knot.fd_listen);
                            printf("ACCEPTED FD:\t%d\n", client_knot.fd_aux);
                        }
                        else
                        {
                            client_knot.fd_aux = acceptTCP(&client_knot.fd_listen);
                            printf("ACCEPTED FD:\t%d\n", client_knot.fd_aux);
                            
                        }
                    }


                    
                    break;
            }
        }


        





        
    }    

    exit(0);
}






