//tcp test
#include "tcp.h"

int main(void) {

    //short key_in;
    char ip_in[16], port_in[8];
    int fd;

    ssize_t nbytes, nleft, nwritten, nread;
    char *ptr, buffer[128+1];

    strcpy(ip_in, "193.136.138.142");
    strcpy(port_in,"58001");

    fd = new_conection( &ip_in, &port_in); /*conecting with client*/

    //TCP write and read
    ptr = strcpy(buffer, "Hello!\n");
    nbytes = strlen(ptr);
    printf("nbytes: %zd \n", nbytes);
    nleft = nbytes;

    while (nleft>0) {nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) /*error*/ exit(1);
        nleft -= nwritten;
        ptr += nwritten;}
    nleft = nbytes; ptr = buffer;

    while (nleft > 0) {nread = read(fd, ptr, nleft);
        if (nread == -1) /*error*/ exit(1);
        else if (nread == 0) break; /*closed by peer*/
        nleft -= nread;
        ptr += nread;}

    nread = nbytes - nleft;


    buffer [nread] = '\0';
    printf ("echo: %s\n", buffer);
    close_conection (&fd);
    printf("FD: %i", fd);
    exit(0);

}

int new_conection(char ip_in, char port_in){
    struct addrinfo hints, *res;
    int fd, n, errorcode;

    //TCP socket and connect
    fd = socket (AF_INET, SOCK_STREAM, 0);    //TCP socket
    /*file descriptor print*/ printf("FD: %i \n", fd);
    if (fd == -1) exit(1); //error

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;    //IPv4
    hints.ai_socktype = SOCK_STREAM;  //TCP socket

    errorcode = getaddrinfo (ip_in, port_in, &hints, &res);
    if (errorcode != 0) /*error*/ exit(1);

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) /*error*/ exit(1);

    return (fd);
}

void close_conection (int *fd){
    close(*fd);
}
