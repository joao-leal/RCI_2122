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

void Show(knot *k)
{
    printf("\e[1;1H\e[2J");

    printf("KNOT: %d\tIP: %s\tPORT: %s\n", k->self_key, k->self_IP, k->self_Port);

    //if the knot doesn't have a succesor then it's alone in the ring!
    if(!strcmp(k->succ_IP, ""))
    {
        printf("KNOT IS ALONE IN THE RING.\n\n");
        return;
    }
    else
        printf("SUCESSOR: %d\tIP: %s\tPORT: %s\n", k->succ_key, k->succ_IP, k->succ_Port);

    //from here the knot can't be alone in the ring
    printf("PREDECESSOR: %d\tIP: %s\tPORT: %s\n", k->pred_key, k->pred_IP, k->pred_Port);

    //if there is a shortcut somewhere it's also displayed
    if(strcmp(k->short_IP, ""))
        printf("SHORTCUT: %d\tIP: %s\tPORT: %s\n", k->short_key, k->short_IP, k->short_Port);

}

void Bentry(knot *k, short *boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\nBOOT: \t %d\nIP: \t %s\nPort: \t %s\n", *boot, boot_IP, boot_Port);
    
}

void Pentry(knot *k, short *pred, char *pred_IP, char *pred_Port)
{
    //The entering knot has to send "SELF i i.IP i.port\n" to 'pred' via TCP
    short key;
    char i_IP[IP_SIZE] = "", i_Port[PORT_SIZE] = "", self_m[128] = "";

    key = k->self_key;
    strcpy(i_IP, k->self_IP);
    strcpy(i_Port, k->self_Port);

    //Its predecessor is now 'pred'
    k->pred_key = *pred;
    strcpy(k->pred_IP, pred_IP);
    strcpy(k->pred_Port, pred_Port);

    sprintf(self_m, "SELF %d %s %s\n", key, i_IP, i_Port);

    //Open TCP connection with predecessor
    k->fd_pred = connectTCP(k->pred_IP, k->pred_Port);
    printf("CONNECTED\n");
    writeTCP(&k->fd_pred, self_m);
    closeTCP(&k->fd_pred);



}