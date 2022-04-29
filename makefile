#   Compiler, Compiler Flags
CC = gcc
CFLAGS1 =-g -Wall
CFLAGS2=-Wall -std=c99 -O3


#   Sources
SOURCES = ring.c ring_interface.c net.c

#   Objects  ('Make' automatically compiles .c to .o)
OBJECTS_A = ring.o ring_interface.o net.o
OBJECTS_B = udp.o


.c.o:
	$(CC) $(CFLAGS1) -c -o $@ $<

ring: $(OBJECTS_A)
	$(CC) $(CFLAGS1) -o $@ $(OBJECTS_A)

ring_final: $(OBJECTS_A)
	$(CC) $(CFLAGS2) -o $@ $(OBJECTS_A)

udp_send: $(OBJECTS_B)
	$(CC) $(CFLAGS1) -o $@ $(OBJECTS_B)

clean:
	rm -f *.o *.~ *.gch