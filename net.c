#include "net.h"


void newUDP_server(knot *k)
{
    //Creates a new UDP server
    struct addrinfo hints, *res;
    int fd, errorcode;
    ssize_t n, nread;
    struct sockaddr addr;
    socklen_t addrlen;
    char buffer[128];

    //Creates a UDP socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
        exit(1);

    //Inicializes hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //ipv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket
    hints.ai_flags = AI_PASSIVE;

    if((errorcode = getaddrinfo(NULL, k->self_Port, &hints, &res)) != 0)
        exit(1);

    if(bind(fd, res->ai_addr, res->ai_addrlen) == -1)
        exit(1);

    printf("Listening on port %s...\n", k->self_Port);

    while(1)
    {
        addrlen = sizeof(addr);

        nread = recvfrom(fd, buffer, 128, 0, &addr, &addrlen);
        if(nread == -1)
            exit(1);

        //echo server
        write(1, "received: ", 10);
        write(1, buffer, nread);
        write(1 , "\n", 1);
        n = sendto(fd, buffer, nread, 0, &addr, addrlen);
        if(n == -1)
            exit(1);
        
        //if it receives a message saying "close", stops receiving and closes fd
        if(!strcmp(buffer, "close"))
            break;
    }

    freeaddrinfo(res);
    close(fd);
}

