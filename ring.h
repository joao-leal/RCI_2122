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
#include <errno.h>

#define RING_SIZE 32
#define COMMAND_LENGTH 16
#define IP_SIZE 50
#define PORT_SIZE 6
#define MAX_MESSAGE_LENGTH 128
#define MY_INT_IP "192.168.1.80"
#define MY_EXT_IP "85.243.0.129"
#define max(A,B) ((A)>=(B)?(A):(B))


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
    int fd_pred;

    //Successor
    short succ_key;
    char succ_IP[IP_SIZE];
    char succ_Port[PORT_SIZE];
    int fd_succ;

    //Shortcut
    short short_key;
    char short_IP[IP_SIZE];
    char short_Port[PORT_SIZE];
    int fd_short;

    //Question
    short find_key;
    char question_IP[IP_SIZE];
    char question_Port[PORT_SIZE];
    int fd_question;
    int seq[100];

    //File Descriptors
    int fd_listen;
    int fd_UDP; 
    int fd_aux;

} knot;


typedef struct
{
    short knots;
    short keys[32]; 

} ring;

#include "net.h"


//"\e[1;1H\e[2J" clears console
// 94.62.180.116
// 94.62.180.


//Functions Definition
void menu_dsp();
void new(knot *); //Creates a new ring with only 1 knot
void bentry(knot *, short *, char *, char *); //Enters a new knot ?
void pentry(knot *, short *, char *, char *); // Enters a new knot knowing its predecessor
void chord(short, char *, char *);
void echord();
void show(knot *);
void Find(short k);
void leave();

void add_active_fds(knot *, fd_set *, int *);
void close_all(knot *);

void msg_create(char *, char *, knot *);
void msg_handle(char *, knot *);


#endif