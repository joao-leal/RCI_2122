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

    printf("CREATING NEW RING...\n");
    
    newUDP_server(host);
       
    
}


void Show(knot *the_knot)
{
    printf("\e[1;1H\e[2J");

    printf("KNOT: %d\tIP: %s\tPORT: %s\n", the_knot->self_key, the_knot->self_IP, the_knot->self_Port);

    //if the knot doesn't have a succesor then it's alone in the ring!
    if(!strcmp(the_knot->succ_IP, ""))
    {
        printf("KNOT IS ALONE IN THE RING.\n\n");
        return;
    }
    else
        printf("SUCESSOR: %d\tIP: %s\tPORT: %s\n", the_knot->succ_key, the_knot->succ_IP, the_knot->succ_Port);

    //from here the knot can't be alone in the ring
    printf("PREDECESSOR: %d\tIP: %s\tPORT: %s\n", the_knot->pred_key, the_knot->pred_IP, the_knot->pred_Port);

    //if there is a shortcut somewhere it's also displayed
    if(strcmp(the_knot->short_IP, ""))
        printf("SHORTCUT: %d\tIP: %s\tPORT: %s\n", the_knot->short_key, the_knot->short_IP, the_knot->short_Port);

}

void Bentry(short *boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\nBOOT: \t %d\nIP: \t %s\nPort: \t %s\n", *boot, boot_IP, boot_Port);
    
}


void Pentry(short *pred, char *pred_IP, char *pred_Port)
{
    printf("Doing something with:\nPRED: \t %d\nIP: \t %s\nPort: \t %s\n", *pred, pred_IP, pred_Port);
}