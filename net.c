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
    hints.ai_family = AF_INET; //IPv4
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

        
        write(1, "received: ", 10);
        write(1, buffer, nread);
        write(1 , "\n", 1);

        //echo server (sends only read bytes)
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

int newTCP(char *i_IP, char *i_Port)
{
    //short key_in;
    int fd, n, errorcode;
    struct addrinfo hints, *res;

    //TCP socket and connect
    fd = socket(AF_INET, SOCK_STREAM, 0);    //TCP socket

    printf("FD: %i \n", fd); //file descriptor print
    if(fd == -1) exit(1); //error

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;    //IPv4
    hints.ai_socktype = SOCK_STREAM;  //TCP socket

    errorcode = getaddrinfo(i_IP, i_Port, &hints, &res);
    if (errorcode != 0) //error 
        exit(1);

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) //error
        exit(1);


/* 
    while (nleft > 0) {nread = read(fd, ptr, nleft);
    if (nread == -1) //error
        exit(1);
    else if (nread == 0) break; //closed by peer
    nleft -= nread;
    ptr += nread;}

    nread = nbytes - nleft;

    buffer [nread] = '\0';
    printf ("echo: %s\n", buffer);
    printf("FD: %i", fd); 
*/

    return fd;

}


void writeTCP(int *fd, char *message)
{
    ssize_t nbytes, nleft, nwritten;

    message = strcpy(buffer, "Hello!\n");
    nbytes = strlen(ptr);
    printf("nbytes: %zd \n", nbytes);
    nleft = nbytes;

    while (nleft>0) {nwritten = write(fd, ptr, nleft);
    if (nwritten <= 0) /*error*/ exit(1);
    nleft -= nwritten;
    ptr += nwritten;}
    nleft = nbytes; ptr = buffer;
}

void closeTCP (int *fd){
    close(*fd);
}