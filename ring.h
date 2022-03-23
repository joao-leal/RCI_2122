#ifndef RING
#define RING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


//Functions Definition
void New(); //Creates a new ring with only 1 knot
void Bentry(int boot, char *IP, char *Port); //Enters a new knot ?
void Pentry(int pred, char *pred_IP, char *pred_Port); // Enters a new knot knowing its predecessor
void Chord(int i, char *i_IP, char *i_Port);
void Echord();
void Show();
void Find(int k);
void Leave();


#endif