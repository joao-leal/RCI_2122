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

    printf("CREATING NEW RING...\n");
    
    newUDP_server(host);
       
    
}


void Bentry(short *boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\nBOOT: \t %d\nIP: \t %s\nPort: \t %s\n", *boot, boot_IP, boot_Port);
    
    return;
}


void Pentry(short *pred, char *pred_IP, char *pred_Port)
{
    printf("Doing something with:\n key: %hn\nIP: %s\nPort: %s\n", pred, pred_IP, pred_Port);
}