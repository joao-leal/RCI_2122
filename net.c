#include "net.h"


void newUDP(knot *k)
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
}

