//tcp test
#include "net.h"

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



