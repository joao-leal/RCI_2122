#ifndef NET
#define NET

#include "ring.h"


#define MAX_BACKLOG 4

int newUDP(knot *);
int listenTCP(char *);
int connectTCP(char *, char*);
int acceptTCP(int *);
void writeTCP(int *, char *);
void readTCP(int *, char *);
void closeTCP(int *);

#endif