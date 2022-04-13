#include "ring.h"

void New(knot *host)
{
    //Creates a knot which is its own predecessor and successor

    host->pred_key = host->succ_key = host->self_key;
    strcpy(host->pred_IP, host->self_IP);
    strcpy(host->pred_Port, host->self_Port);

    printf("CREATING NEW RING...\n");
    
    host->fd_UDP = newUDP(host);
    printf("UDP FD:\t%d\n", host->fd_UDP);

    host->fd_listen = listenTCP(host->self_Port);
    printf("TCP LISTEN FD:\t%d\n", host->fd_listen);

    
}

void Show(knot *node)
{
    printf("\e[1;1H\e[2J");

    printf("KNOT: %d\tIP: %s\tPORT: %s\n", node->self_key, node->self_IP, node->self_Port);

    //if the knot doesn't have a succesor then it's alone in the ring!
    if(!strcmp(node->succ_IP, ""))
    {
        printf("KNOT IS ALONE IN THE RING.\n\n");
        return;
    }
    else
        printf("SUCESSOR: %d\tIP: %s\tPORT: %s\n", node->succ_key, node->succ_IP, node->succ_Port);

    //from here the knot can't be alone in the ring
    printf("PREDECESSOR: %d\tIP: %s\tPORT: %s\n", node->pred_key, node->pred_IP, node->pred_Port);

    //if there is a shortcut somewhere it's also displayed
    if(strcmp(node->short_IP, ""))
        printf("SHORTCUT: %d\tIP: %s\tPORT: %s\n", node->short_key, node->short_IP, node->short_Port);

}

void Bentry(knot *node, short *boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\nBOOT: \t %d\nIP: \t %s\nPort: \t %s\n", *boot, boot_IP, boot_Port);
    
}

void Pentry(knot *node, short *pred, char *pred_IP, char *pred_Port)
{
    //The entering knot has to send "SELF i i.IP i.port\n" to 'pred' via TCP

    char i_IP[IP_SIZE] = "", i_Port[PORT_SIZE] = "", msg[MAX_MESSAGE_LENGTH] = "";

    strcpy(i_IP, node->self_IP);
    strcpy(i_Port, node->self_Port);

    //Its predecessor is now 'pred'
    node->pred_key = *pred;
    strcpy(node->pred_IP, pred_IP);
    strcpy(node->pred_Port, pred_Port);

    //Open TCP connection with predecessor
    node->fd_pred = connectTCP(node->pred_IP, node->pred_Port);
    printf("CONNECTED\n");

    //Create SELF message and send it
    Msg_Create(msg, 0, node);
    writeTCP(&node->fd_pred, msg);

    printf("SENT:\t%s\n", msg);
}

void Msg_Create(char *msg, int mode, knot *node)
{
    switch(mode)
    {
        //Type SELF
        case 0:
            sprintf(msg, "SELF %d %s %s\n", node->self_key, node->self_IP, node->self_Port);
            break;
        //Type PRED
        case 1:
            sprintf(msg, "SELF %d %s %s\n", node->self_key, node->self_IP, node->self_Port);
            break;
    }    

}

void Msg_Handle(char *msg, knot *node, char *retmsg)
{
    if(!strcmp("SELF", strtok(msg, " ")))
    {
        //Node is getting information about its
        //new successor

        printf("HANDLING SELF MSG...\n");

        if(node->succ_key != 0)
        {
            //It has to inform its old successor of
            //its new predecessor (the old successor's)

            //If the node doesn't have already a TCP 
            //connection to its successor then it is created

            if(node->fd_succ == 0)
            {
                node->fd_succ = connectTCP(node->succ_IP, node->succ_Port);
                if(node->fd_succ == -1)
                {
                    printf("ERROR: SUCC CONNECT MSG_HANDLE\n");
                    exit(1);
                }
            }

            sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);

            //Creates a PRED message to send to it
            Msg_Create(retmsg, 1, node);

            writeTCP(&node->fd_succ, retmsg);

            //It can close its connection to the previous
            //succ, since it isn't no more
            closeTCP(&node->fd_succ);

        }
        else
        {
            sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
        }

        

    }
}