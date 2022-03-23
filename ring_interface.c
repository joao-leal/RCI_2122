#include "ring.h"
#include "net.h"

void New(knot *host, short i, char *i_IP, char *i_Port)
{
    //Creates a knot which is its own predecessor and successor

    host->self_key = i;
    host->pred_key = i;
    host->succ_key = i;
    strcpy(host->self_IP, i_IP);
    strcpy(host->self_Port, i_Port);
    strcpy(host->pred_IP, i_IP);
    strcpy(host->pred_Port, i_Port);
    strcpy(host->succ_IP, i_IP);
    strcpy(host->succ_Port, i_Port);

    newUDP(host);
       
    printf("CREATING NEW RING...\n");
}


void Bentry(short boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\n key: %d\nIP: %s\nPort: %s", boot, boot_IP, boot_Port);
}


void Pentry(short pred, char *pred_IP, char *pred_Port)
{
    printf("Doing something with:\n key: %d\nIP: %s\nPort: %s", pred, pred_IP, pred_Port);
}