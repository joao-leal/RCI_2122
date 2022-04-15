#ifndef NET_
#define NET_

#include "ring.h"


#define MAX_BACKLOG 4

int new_udp(knot *, struct sockaddr_in *, socklen_t *);
void read_udp(int *, char *);
int listen_tcp(char *);
int connect_tcp(char *, char*);
int accept_tcp(int *);
void write_tcp(int *, char *);
void read_tcp(int *, char *);
void close_tcp(int *);

#endif