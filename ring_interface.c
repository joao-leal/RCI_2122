#include "ring.h"


void new(knot *host)
{
    //Creates a knot which is its own predecessor and successor

    host->pred_key = host->self_key;
    strcpy(host->pred_IP, host->self_IP);
    strcpy(host->pred_Port, host->self_Port);

    printf("CREATING NEW RING...\n");
    

    
}

void show(knot *node)
{
    printf("\e[1;1H\e[2J");

    printf("NODE:\t %d\tIP: %s\tPORT: %s\n", node->self_key, node->self_IP, node->self_Port);

    //if the knot doesn't have a succesor then it's alone in the ring!
    if(!strcmp(node->succ_IP, "") && !strcmp(node->pred_IP, ""))
    {
        printf("NODE IS ALONE IN THE RING.\n\n");
        return;
    }
    else if(strcmp(node->succ_IP, ""))
        printf("SUCCESSOR:\t %d\tIP: %s\tPORT: %s\n", node->succ_key, node->succ_IP, node->succ_Port);

    //from here the knot can't be alone in the ring
    printf("PREDECESSOR:\t %d\tIP: %s\tPORT: %s\n", node->pred_key, node->pred_IP, node->pred_Port);

    //if there is a shortcut somewhere it's also displayed
    if(strcmp(node->short_IP, ""))
        printf("SHORTCUT:\t %d\tIP: %s\tPORT: %s\n", node->short_key, node->short_IP, node->short_Port);

}

void bentry(knot *node, short *boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\nBOOT: \t %d\nIP: \t %s\nPort: \t %s\n", *boot, boot_IP, boot_Port);
    
}

void pentry(knot *node, short *pred, char *pred_IP, char *pred_Port)
{
    //The entering knot has to send "SELF i i.IP i.port\n" to 'pred' via TCP

    char i_IP[IP_SIZE] = "", i_Port[PORT_SIZE] = "", msg[MAX_MESSAGE_LENGTH] = "";

    strcpy(i_IP, pred_IP);
    strcpy(i_Port, pred_Port);

    printf("PENTRY: %s - %s\n", i_IP, i_Port);

    //Open TCP connection with predecessor
    node->fd_pred = connect_tcp(i_IP, i_Port);
    if(node->fd_pred == -1)
    {
        perror("pentry connect:");
        exit(1);
    }
    printf("CONNECTED\n");

    //Its predecessor is now 'pred'
    node->pred_key = *pred;
    strcpy(node->pred_IP, pred_IP);
    strcpy(node->pred_Port, pred_Port);
    sleep(1);

    //Create SELF message and send it
    msg_create(msg, "SELF", node);

    write_tcp(&node->fd_pred, msg);

}

void add_active_fds(knot *node, fd_set *read_fds, int *max_fd)
{
    *max_fd = 0;

    FD_ZERO(read_fds);

    FD_SET(STDIN_FILENO, read_fds);
    *max_fd = STDIN_FILENO;

    if(node->fd_listen)
    {
        FD_SET(node->fd_listen, read_fds);
        *max_fd = max(node->fd_listen, *max_fd);
    }

    if(node->fd_aux)
    {
        FD_SET(node->fd_aux, read_fds);
        *max_fd = max(node->fd_aux, *max_fd);
    }

    if(node->fd_pred)
    {
        FD_SET(node->fd_pred, read_fds);
        *max_fd = max(node->fd_pred, *max_fd);
    }

    if(node->fd_succ)
    {
        FD_SET(node->fd_succ, read_fds);
        *max_fd = max(node->fd_succ, *max_fd);
    }

    if(node->fd_short)
    {
        FD_SET(node->fd_short, read_fds);
        *max_fd = max(node->fd_short, *max_fd);
    }

    if(node->fd_UDP)
    {
        FD_SET(node->fd_UDP, read_fds);
        *max_fd = max(node->fd_UDP, *max_fd);
    }
}

void close_all(knot *node)
{
    if(node->fd_listen)
        close_tcp(&node->fd_listen);
    
    if(node->fd_aux)
        close_tcp(&node->fd_aux);
    
    if(node->fd_pred)
        close_tcp(&node->fd_pred);    
    
    if(node->fd_succ)
        close_tcp(&node->fd_succ);
    
    if(node->fd_short)
        close_tcp(&node->fd_short);

    if(node->fd_UDP)
        close(node->fd_UDP);
}

void msg_create(char *msg, char mode[5], knot *node)
{
    //Type SELF
    if(!strcmp(mode, "SELF"))
    {
        sprintf(msg, "SELF %d %s %s\n", node->self_key, node->self_IP, node->self_Port);
    }
    //Type PRED
    else if(!strcmp(mode, "PRED"))
        sprintf(msg, "PRED %d %s %s\n", node->succ_key, node->succ_IP, node->succ_Port);
    //Type PRED for leave
    else if(!strcmp(mode, "PREDL"))
    {
        sprintf(msg, "PRED %d %s %s\n", node->pred_key, node->pred_IP, node->pred_Port);
    }
}

void msg_handle(char *msg, knot *node)
{
    char buffer[MAX_MESSAGE_LENGTH];
    strcpy(buffer, msg);

    if(!strcmp("SELF", strtok(buffer, " ")))
    {
        char retmsg[MAX_MESSAGE_LENGTH];
        short recv_key = 0;

        sscanf(msg, "%*s %hd", &recv_key);
        printf("HANDLING SELF MSG...\n");

        printf("RECVD KEY: %hd \t SELF KEY: %hd \t PRED KEY: %d \t SUCC KEY: %hd\n", recv_key, node->self_key, node->pred_key, node->succ_key);

        //Node is trying to enter with a key equal 
        //to mine!
        if(recv_key == node->self_key)
        {
            write_tcp(&node->fd_aux, "Same key as mine!\n");
            sleep(1);
            close_tcp(&node->fd_aux);
        }
        else if(recv_key > node->self_key) 
        {
            //I'm PRED to node but alone in the ring
            if(node->self_key == node->pred_key && node->succ_key == -1)
            {
                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;

                //PRED = SUCC
                node->pred_key = node->succ_key;
                strcpy(node->pred_IP, node->succ_IP);
                strcpy(node->pred_Port, node->succ_Port);

                //Send SELF to PRED (through fd_succ)
                msg_create(retmsg, "SELF", node);

                write_tcp(&node->fd_succ, retmsg);
                return;
            }
            else if(node->succ_key != -1)
            //I'm PRED to node entering normal ring
            {
                int prev_succ_fd = -1;

                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                prev_succ_fd = node->fd_succ;
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;

                //Send PRED to previous SUCC
                msg_create(retmsg, "PRED", node);

                write_tcp(&prev_succ_fd, retmsg);

                //sleep(5);

                //Close previous SUCC conn.
                //close_tcp(&prev_succ_fd);

                return;
            }
            else if(node->succ_key == -1)
            //I'm receiving SUCC info
            {
                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;

                return;
            }
            else if(recv_key == node->succ_key)
            //Node can't enter because it has the same
            //key as my SUCC
            {
                //Can't connect, bye!
                write_tcp(&node->fd_aux, "Same key as my successor, try another!\n");
                sleep(1);
                close_tcp(&node->fd_aux);
            }
            else if(recv_key > node->succ_key)
            //It means my SUCC wants to leave
            {
                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;
            }
        }
        else if(recv_key < node->self_key)
        {
            //I'm the node entering the ring with only 1
            //node
            if(recv_key == node->pred_key)
            {
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                node->fd_succ = node->fd_pred;

                return;
            }
            //My new SUCC wants to inform me
            else if(node->succ_key == -1)
            {   
                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;
            }
            else if(recv_key < node->succ_key)
            //Node wants to enter through the end
            //(I'm the biggest key node))
            {
                //Normal entry(?)
                int prev_succ_fd;

                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                prev_succ_fd = node->fd_succ;
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;
                

                //Send PRED to prev. SUCC
                msg_create(retmsg, "PRED", node);

                write_tcp(&prev_succ_fd, retmsg);


                //Close conn. inbetween
                
                //close_tcp(&prev_succ_fd);

            }
            else if(recv_key < node->pred_key)
            {
                //Update SUCC
                sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
                node->fd_succ = node->fd_aux;
                node->fd_aux = -1;

                return;

            }
            else
            //This can't happen! I can't be PRED to a
            //key before me!
            {
                //Can't have a key lower than mine, bye!
                printf("KEY IS LOWER THAN MINE\n");
                write_tcp(&node->fd_aux, "Your key is lower than mine!\n");
                sleep(3);
                close_tcp(&node->fd_aux);
                node->fd_aux = -1;

                return;
            }
        }
        
    }//SELF

    else if(!strcmp("PRED", strtok(buffer, " ")))
    {
        char retmsg[MAX_MESSAGE_LENGTH];
        int prev_pred_fd = 0;
        short recv_key;

        printf("HANDLING PRED MSG...\n");

        sscanf(msg, "%*s %hd", &recv_key);

        if(recv_key == node->self_key)
        //I'm the last one!
        {
            //Update SUCC & PRED
            sscanf(msg, "%*s %hd %s %s", &node->pred_key, node->pred_IP, node->pred_Port);
            prev_pred_fd = node->fd_pred;

            node->succ_key = -1;
            strcpy(node->succ_IP, "");  
            strcpy(node->succ_Port, "");
            close_tcp(&node->fd_pred);
            close_tcp(&node->fd_succ);         


            return;
        }
        else
        {
            //Update PRED
            sscanf(msg, "%*s %hd %s %s", &node->pred_key, node->pred_IP, node->pred_Port);
            prev_pred_fd = node->fd_pred;

            //Send SELF to new PRED
            node->fd_pred = connect_tcp(node->pred_IP, node->pred_Port);
            if(node->fd_pred == -1)
            {
                perror("CONNECT PRED MSG:");
                exit(1);
            }

            msg_create(retmsg, "SELF", node);

            write_tcp(&node->fd_pred, retmsg);

            
            //Close conn. with prev. PRED
            //sleep(5);
            //close_tcp(&prev_pred_fd);

            return;

        }

    }

    else if(!strcmp("LEAVE", strtok(buffer, " ")))
    //Abstraction workaround, tiddier than sending
    //messages from main
    {
        char retmsg[MAX_MESSAGE_LENGTH];
        int recv_key = -1;

        sscanf(msg, "%*s %d", &recv_key);

        if(node->succ_key == node->pred_key && node->succ_key != -1 && (recv_key == node->pred_key || recv_key == node->succ_key))
        //Last 2 nodes in ring (Leave)
        {

        }
        else
        {
            msg_create(retmsg, "PREDL", node);

            write_tcp(&node->fd_succ, retmsg);
            

            //Close all conections
            sleep(5);
            close_all(node);

        }
        
        //Send PRED to SUCC
    }
    else
    {
        puts(msg);
        return;
    }

}