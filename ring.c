#include "ring.h"


int main(int argc, char * argv[])
{

    knot node;
    int max_fd = 0;
    fd_set read_fds;
    //enum {idle, busy} state;

    //Initializes knot structure
    memset(&node, 0, sizeof(knot));
    node.pred_key = -1;
    node.succ_key = -1;
    node.short_key = -1;
    node.fd_aux = -1;
    node.fd_pred = -1;
    node.fd_succ = -1;
    for (int p=0; p < 100; p++){
        node.seq [p] = 0;
    }

    //Prevents SIGPIPE Signal
    struct sigaction act;
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    sigaction(SIGPIPE,&act,NULL);


    if(atoi(argv[1]) > 31 || atoi(argv[1]) < 0)
    {
        printf("Key must be within ring limits [0:31]\n");
        exit(1);
    }
    node.self_key = atoi(argv[1]);

    strcpy(node.self_IP, argv[2]);
    strcpy(node.self_Port, argv[3]);

    printf("KEY: \t %d\nIP: \t %s\nPORT: \t %s\n", node.self_key, node.self_IP, node.self_Port);
    
    if(!node.fd_UDP)
        node.fd_UDP = new_udp(node.self_Port);

    if(!node.fd_listen)
        node.fd_listen = listen_tcp(node.self_Port);
    //User Interface
    
    while(1)
    {

        char input[MAX_MESSAGE_LENGTH] = "", command[COMMAND_LENGTH] = "";
        int counter = 0;

        //struct timeval tv;

        //tv.tv_sec = 120;

        //printf("LOOP\n");
        
        
        //Adds the active file descriptors for TCP & UDP connections and for the STDIN to the read set
        add_active_fds(&node, &read_fds, &max_fd);
    
        //printf("PRED_FD: %d\tSUCC_FD: %d\tLISTEN_FD: %d\n", node.fd_pred, node.fd_succ, node.fd_listen);

        counter = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if(counter <= 0)
        {
            perror("select");
            exit(1);
        }
        

        /************************************/
        /*        v     CLIENT     v        */
        /************************************/

        if(FD_ISSET(STDIN_FILENO, &read_fds))
        {
            fgets(input, MAX_MESSAGE_LENGTH, stdin);
            sscanf(input, "%s", command);
        
            // printf("THIS: %s\n", command);

            if(!strcmp("new", command) || !strcmp("n", command))
            {
                //printf("\e[1;1H\e[2J");
                new(&node);
            }

                

            else if(!strcmp("bentry", command) || !strcmp("b", command))
            {
                short boot;
                char boot_IP[16] = "", boot_Port[6] = "";

                //printf("\e[1;1H\e[2J");
                sscanf(input, "%*s %hi %s %s", &boot, boot_IP, boot_Port);
                //printf("BOOT: \t %d\nIP: \t %s\nPORT: \t %s\n", boot, boot_IP, boot_Port);

                bentry(&node, &boot, boot_IP, boot_Port);
            }
            else if(!strcmp("pentry", command) || !strcmp("p", command))
            {
                short pred;
                char pred_IP[16] = "", pred_Port[6] = "";

                sscanf(input, "%*s %hi %s %s", &pred, pred_IP, pred_Port);
                //printf("\e[1;1H\e[2J");
                pentry(&node, &pred, pred_IP, pred_Port);
                printf("EXITED PENTRY\n");
                

                
            }
            else if(!strcmp("show", command) || !strcmp("s", command))
            {
                show(&node);
            }

            else if(!strcmp("find", command) || !strcmp("f", command))
            {
                msg_create(input, "FIND", &node);
            }

            else if(!strcmp("leave", command) || !strcmp("l", command))
            {
                msg_handle("LEAVE", &node);

                sleep(2);
                close_tcp(&node.fd_pred);
                node.fd_pred = -1;

                close_tcp(&node.fd_succ);
                node.fd_succ = -1;

                //Resets PRED and SUCC
                node.pred_key = -1;
                node.succ_key = -1;

                strcpy(node.pred_IP, "");
                strcpy(node.pred_Port, "");
                strcpy(node.succ_IP, "");
                strcpy(node.succ_Port, "");

                show(&node);

                continue;
            }
            else if(!strcmp("exit", command) || !strcmp("e", command))
            {
                close_all(&node);
                printf("EXITING\n");
                exit(0);
            }
        }//STDIN

        /************************************/
        /*        v     SERVER     v        */
        /************************************/

        //Incoming TCP request
        if(node.fd_listen && FD_ISSET(node.fd_listen, &read_fds))
        {
            //Accept incoming request
            printf("ACCEPTING\n");
            sleep(1);

            node.fd_aux = accept_tcp(&node.fd_listen);
            printf("CONNECTED\n");

            
        }//fd_listen

        //Incoming message from some node
        if(node.fd_aux > 0 && FD_ISSET(node.fd_aux, &read_fds))
        {
            char buffer[MAX_MESSAGE_LENGTH];

            read_tcp(&node.fd_aux, buffer);
            printf("RECEIVED:\t%s\n", buffer);

            //We have to handle the message
            msg_handle(buffer, &node);

            strcpy(buffer, "");

            /* FD_CLR(node.fd_aux, &read_fds);
            node.fd_aux = 0; */

            show(&node);

            // sleep(2);
            //FD_ZERO(&read_fds);
            continue;


        }//fd_aux

        //Incoming message from PRED
        if(node.fd_pred > 0 && FD_ISSET(node.fd_pred, &read_fds))
        {
            char buffer[MAX_MESSAGE_LENGTH];

            printf("Incoming from PRED\n");

            read_tcp(&node.fd_pred, buffer);
            printf("RECEIVED FROM PRED:\t%s\n", buffer);
            if(strstr(buffer, "!"))
            {
                printf("Bye!\n");
                close_tcp(&node.fd_pred);
                exit(0);
            }

            //We have to handle the message
            msg_handle(buffer, &node);
        
            show(&node);
           
            // sleep(2);

            //FD_ZERO(&read_fds);


            continue;

        }

        //Incoming message from SUCC
        if(node.fd_succ > 0 && FD_ISSET(node.fd_succ, &read_fds))
        {
            char buffer[MAX_MESSAGE_LENGTH];

            //printf("Incoming from SUCC\n");
            read_tcp(&node.fd_succ, buffer);
            //printf("RECEIVED FROM SUCC:\t%s\n", buffer);

            //We have to handle the message
            msg_handle(buffer, &node); 



            show(&node);

            // sleep(2);

            //FD_ZERO(&read_fds);

            continue;

        }

        if(node.fd_UDP > 0 && FD_ISSET(node.fd_UDP, &read_fds))
        {

        }
        /* if(node.fd_short && FD_ISSET(node.fd_short, &read_fds))
        {
            continue;
        } */

    }//while

    close_all(&node);

    exit(0);
}






