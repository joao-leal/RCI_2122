#include "ring.h"


void new(knot *host)
{
    //Creates a knot which is its own predecessor and successor

    host->pred_key = host->succ_key = host->self_key;
    strcpy(host->pred_IP, host->self_IP);
    strcpy(host->pred_Port, host->self_Port);

    printf("CREATING NEW RING...\n");
    
    host->fd_UDP = new_udp(host);
    printf("UDP FD:\t%d\n", host->fd_UDP);

    host->fd_listen = listen_tcp(host->self_Port);
    printf("TCP LISTEN FD:\t%d\n", host->fd_listen);

    
}

void show(knot *node)
{
    printf("\e[1;1H\e[2J");

    printf("KNOT:\t\t %d\t\tIP: %s\t\tPORT: %s\n", node->self_key, node->self_IP, node->self_Port);

    //if the knot doesn't have a succesor then it's alone in the ring!
    if(!strcmp(node->succ_IP, "") && !strcmp(node->pred_IP, ""))
    {
        printf("KNOT IS ALONE IN THE RING.\n\n");
        return;
    }
    else if(strcmp(node->succ_IP, ""))
        printf("SUCCESSOR:\t %d\t\tIP: %s\t\tPORT: %s\n", node->succ_key, node->succ_IP, node->succ_Port);

    //from here the knot can't be alone in the ring
    printf("PREDECESSOR:\t %d\t\tIP: %s\t\tPORT: %s\n", node->pred_key, node->pred_IP, node->pred_Port);

    //if there is a shortcut somewhere it's also displayed
    if(strcmp(node->short_IP, ""))
        printf("SHORTCUT:\t %d\t\tIP: %s\t\tPORT: %s\n", node->short_key, node->short_IP, node->short_Port);

}

void bentry(knot *node, short *boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\nBOOT: \t %d\nIP: \t %s\nPort: \t %s\n", *boot, boot_IP, boot_Port);
    
}

void pentry(knot *node, short *pred, char *pred_IP, char *pred_Port)
{
    //The entering knot has to send "SELF i i.IP i.port\n" to 'pred' via TCP

    char i_IP[IP_SIZE] = "", i_Port[PORT_SIZE] = "", msg[MAX_MESSAGE_LENGTH] = "";

    strcpy(i_IP, node->self_IP);
    strcpy(i_Port, node->self_Port);

    //Its predecessor is now 'pred'
    node->pred_key = *pred;
    strcpy(node->pred_IP, pred_IP);
    strcpy(node->pred_Port, pred_Port);

    node->fd_listen = listen_tcp(node->self_Port);

    //Open TCP connection with predecessor
    node->fd_pred = connect_tcp(node->pred_IP, node->pred_Port);
    printf("CONNECTED\n");

    //Create SELF message and send it
    msg_create(msg, "SELF", node);
    write_tcp(&node->fd_pred, msg);

    printf("SENT:\t%s\n", msg);
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
        sprintf(msg, "SELF %d %s %s\n", node->self_key, node->self_IP, node->self_Port);

    //Type PRED
    if(!strcmp(mode, "PRED"))
        sprintf(msg, "PRED %d %s %s\n", node->succ_key, node->succ_IP, node->succ_Port);

}

int msg_handle(char *msg, knot *node)
{
    char buffer[MAX_MESSAGE_LENGTH];
    strcpy(buffer, msg);

    if(!strcmp("SELF", strtok(buffer, " ")))
    {
        //Node is getting information about its
        //new successor

        char retmsg[MAX_MESSAGE_LENGTH];

        printf("HANDLING SELF MSG...\n");
        node->fd_succ = node->fd_aux;
        
        if(strlen(node->succ_IP))
        {
            //It has to inform its old successor of
            //its new predecessor (the old successor's)

             //Creates a PRED message to send to it
            msg_create(retmsg, "PRED" , node);

            printf("MESSAGE TO OLD SUCCESSOR:\t%s\n", retmsg);

           /*  node->fd_succ = connect_tcp(node->succ_IP, node->succ_Port);
            if(node->fd_succ == -1)
            {
                printf("ERROR: SUCC CONNECT MSG_HANDLE\n");
                exit(1);
            } */

            write_tcp(&node->fd_succ, retmsg);

            //It can close its connection to the previous
            //succ, since it is no more
            close_tcp(&node->fd_succ);

            sscanf(buffer, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);

            printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);

        }
        else
        {
            //There are only 2 nodes in the ring
            sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);

            //Its pred = succ

            if(node->succ_key && (node->succ_key != node->pred_key))
            {
                node->fd_pred = node->fd_succ;
                node->pred_key = node->succ_key;
                strcpy(node->pred_IP, node->succ_IP);
                strcpy(node->pred_Port, node->succ_Port);

                printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);

                //Has to send SELF to its pred
                msg_create(buffer, "SELF", node);
                write_tcp(&node->fd_pred, buffer);

                printf("SENT: \t%s\n", buffer);
            }
            else
            {
                node->fd_pred = node->fd_succ;
                node->pred_key = node->succ_key;
                strcpy(node->pred_IP, node->succ_IP);
                strcpy(node->pred_Port, node->succ_Port);

                puts(buffer);
                sscanf(buffer, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);

                printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);
            }

            
        }
    }
        return 0;

    if(!strcmp("PRED", strtok(buffer, " ")))
    {
        char retmsg[MAX_MESSAGE_LENGTH];

        printf("HANDLING PRED MSG...\n");

        sscanf(msg, "%*s %hd %s %s", &node->pred_key, node->pred_IP, node->pred_Port);

        if(node->fd_pred)
        node->fd_pred = connect_tcp(node->succ_IP, node->succ_Port);

        if(node->fd_pred == -1)
        {
            printf("BAD CONNECT TO NEW PRED\n");
            exit(1);
        }

        msg_create(retmsg, "SELF", node);
        write_tcp(&node->fd_pred, retmsg);

        printf("SENT: %s", retmsg);

        return 1;

    }


    return -1;
}