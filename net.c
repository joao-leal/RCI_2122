#include "net.h"


int new_udp(char *u_PORT)
{
    //Creates a new UDP server
    struct addrinfo hints, *res;
    int fd, errorcode;
    //ssize_t n, nread;
    //struct sockaddr addr;
    //socklen_t addrlen;
    //char buffer[128];

    //Creates a UDP socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
        exit(1);

    //Inicializes hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket
    hints.ai_flags = AI_PASSIVE;

    if((errorcode = getaddrinfo(NULL, u_PORT, &hints, &res)) != 0)
    {
        perror("GET ADDR");
        exit(1);
    }

    if(bind(fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("BIND UDP");
        exit(1);
    }    

    // printf("Listening on port %s...\n", k->self_Port);

/*     while(1)
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
    } */

    freeaddrinfo(res);

    return fd;
}

void read_udp(int *fd, char *buffer)
{
    socklen_t addrlen;
    struct sockaddr_in addr;
    ssize_t n;

    addrlen = sizeof(addr);

    n = recvfrom(*fd, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr*)&addr, &addrlen);
    if(n == -1)
    {
        perror("recvfrom");
        exit(1);
    }
}

void write_udp(int *fd, char *msg)
{
    socklen_t addrlen;
    struct sockaddr_in addr;
    ssize_t n;

    addrlen = sizeof(addr);

    n = sendto(*fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, addrlen);
    if(n == -1)
    {
        perror("recvfrom");
        exit(1);
    }
}

int listen_tcp(char *Port)
{
    int fd = 0, errorcode, reuse = 1;
    struct addrinfo hints, *res;

    //TCP socket and connect
    fd = socket(AF_INET, SOCK_STREAM, 0);        //TCP socket

    if(fd == -1) exit(1);                        //error

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;                      //IPv4
    hints.ai_socktype = SOCK_STREAM;                //TCP socket
    hints.ai_flags = AI_PASSIVE;                    //TCP Socket
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

    errorcode = getaddrinfo(MY_INT_IP, Port, &hints, &res);
    if (errorcode != 0)
    { //error 
        fprintf(stderr, "GETADDRINFO ERROR: %s\n", strerror(errno));
        exit(1);
    } 
    
    if(bind(fd,res->ai_addr,res->ai_addrlen)==-1) //error
    {
        perror("bind");
        exit(1);
    }
    printf("BIND\n"); 
    
    if(listen(fd, MAX_BACKLOG) == -1)
    {
        exit(1);
    }
    printf("LISTEN\n");


    //freeaddrinfo(res);

    return fd;
}

int connect_tcp(char *i_IP, char *i_Port)
{
    //short key_in;
    int fd, n, errorcode, reuse = 1;
    struct addrinfo hints, *res;

    //TCP socket and connect
    fd = socket(AF_INET, SOCK_STREAM, 0);    //TCP socket

    if(fd == -1) 
        exit(1); //error

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          //IPv4
    hints.ai_socktype = SOCK_STREAM;    //TCP socket
    hints.ai_protocol=IPPROTO_TCP;      //TCP socket
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

    errorcode = getaddrinfo(i_IP, i_Port, &hints, &res);
    if (errorcode != 0)
    { //error 
            
        exit(1);
    } 

    printf("GOT ADDR INFO: %s - %d\n", res->ai_addr->sa_data, res->ai_addrlen);

    
    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if(n == -1) //error
    {
        fprintf(stderr, "CONNECT ERROR: %s\n", strerror(errno));
        exit(1);
    }

    return fd;

}

void write_tcp(int *fd, char *message)
{
   //printf("writing tcp %s\n",message);
    ssize_t nbytes, nwritten;

    nbytes = strlen(message);
    
    //printf("nbytes: %zd \n", nbytes);
    nwritten = write(*fd, message, nbytes);
    if (nwritten <= 0) /*error*/
        exit(1);


}

int accept_tcp(int *fd)
{
    int new_fd, reuse = 1;
    struct sockaddr addr;
    socklen_t addrlen;

    new_fd = accept(*fd, &addr, &addrlen);

    if(new_fd == -1)
    {
        fprintf(stderr, "ACCEPT ERROR: %s\n", strerror(errno));
        printf("ERROR: %d\n", errno);
        exit(1);
    } 
    setsockopt(new_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    return new_fd;
}

void read_tcp(int *fd, char *buffer)
{
    ssize_t nread;
    char aux[MAX_MESSAGE_LENGTH];

    
    do
    {
        nread = read(*fd, buffer, MAX_MESSAGE_LENGTH);
        if(nread == -1) /*error*/
        {
            fprintf(stderr, "READ ERROR: %s\n", strerror(errno));
            exit(1);
        }
        if(nread == 0)
        {
            close(*fd);
            *fd = -1;
            break;
        }
            

        strcat(aux, buffer);
        printf("2");
    }while(!strstr(buffer, "\n"));

    // puts(buffer);
    // puts(aux);
}

void close_tcp(int *fd){
    close(*fd);
}