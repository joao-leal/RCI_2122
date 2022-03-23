#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


#include "ring.h"

void New()
{

  
    printf("CREATING NEW RING...\n");
}


void Bentry(int boot, char *boot_IP, char *boot_Port)
{
    printf("Doing something with:\n key: %d\nIP: %s\nPort: %s", boot, boot_IP, boot_Port);
}
