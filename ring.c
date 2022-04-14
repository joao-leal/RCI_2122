#include "ring.h"

int main(int argc, char * argv[])
{

    knot node;
    int max_fd = 0;
    fd_set read_fds;
    //enum {idle, busy} state;

    //Initializes knot structure
    memset(&node, 0, sizeof(knot));

    //Prevents SIGPIPE Signal
    struct sigaction act;
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    sigaction(SIGPIPE,&act,NULL);


    if(atoi(argv[1]) > 32 || atoi(argv[1]) < 1)
    {
        printf("Key must be within ring limits [1:32]\n");
        exit(1);
    }
    node.self_key = atoi(argv[1]);

    strcpy(node.self_IP, argv[2]);
    strcpy(node.self_Port, argv[3]);

    printf("KEY: \t %d\nIP: \t %s\nPORT: \t %s\n", node.self_key, node.self_IP, node.self_Port);
    

    //User Interface
    
    while(1)
    {

        char input[MAX_MESSAGE_LENGTH] = "", command[COMMAND_LENGTH] = "";
        int counter = 0;

        struct timeval tv;

        tv.tv_sec = 300;

        //printf("LOOP\n");
        
        //Adds the active file descriptors for TCP & UDP connections and for the STDIN to the read set
        add_active_fds(&node, &read_fds, &max_fd);
        

        counter = select(max_fd+1, &read_fds, NULL, NULL, &tv);
        if(counter <= 0)
            exit(1);
        

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
            if(node.pred_key == 0) //Knot is not in ring
            {
                
                
            }
            else
            {
                //Accept incoming request
                printf("ACCEPTING\n");
                node.fd_aux = accept_tcp(node.fd_listen);
                printf("CONNECTED\n");
                printf("ACCEPTED FD:\t%d\n", node.fd_aux);

            }
        }//fd_listen

        //Incoming message from some node
        if(node.fd_aux && FD_ISSET(node.fd_aux, &read_fds))
        {
            char buffer[MAX_MESSAGE_LENGTH];

            read_tcp(&node.fd_aux, buffer);
            printf("RECEIVED:\t%s\n", buffer);

            //We have to handle the message
            msg_handle(buffer, &node);

            FD_CLR(node.fd_aux, &read_fds);
            node.fd_aux = 0;
        }//fd_aux

        //Incoming message from PRED
        if(node.fd_pred && FD_ISSET(node.fd_pred, &read_fds))
        {
            char buffer[MAX_MESSAGE_LENGTH];

            read_tcp(&node.fd_pred, buffer);
            printf("RECEIVED:\t%s\n", buffer);

            //We have to handle the message
            msg_handle(buffer, &node);
        
        }

        //Incoming message from SUCC
        if(node.fd_succ && FD_ISSET(node.fd_short, &read_fds))
        {
            /* char buffer[MAX_MESSAGE_LENGTH];

            read_tcp(&node.fd_aux, buffer);
            printf("RECEIVED:\t%s\n", buffer);

            //We have to handle the message
            msg_handle(buffer, &node); */
        }

        if(node.fd_short && FD_ISSET(node.fd_short, &read_fds))
        {
        }

    }//while

    close_all(&node);

    exit(0);
}






