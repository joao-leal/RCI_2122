#ifndef RING
#define RING


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

#define RING_SIZE 32
#define IP_SIZE 16
#define PORT_SIZE 6


//Knot Structure
typedef struct
{
    //Self
    short self_key;
    char self_IP[IP_SIZE];
    char self_Port[PORT_SIZE];   

    //Predecessor
    short pred_key;
    char pred_IP[IP_SIZE];
    char pred_Port[PORT_SIZE];

    //Successor
    short succ_key;
    char succ_IP[IP_SIZE];
    char succ_Port[PORT_SIZE];

} knot;


//Functions Definition
void New(knot *, short , char *, char *); //Creates a new ring with only 1 knot
void Bentry(short *, char *, char *); //Enters a new knot ?
void Pentry(short *, char *, char *); // Enters a new knot knowing its predecessor
void Chord(short, char *, char *);
void Echord();
void Show();
void Find(short k);
void Leave();


#endif