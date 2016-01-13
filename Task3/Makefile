CC=gcc
CFLAGS=-g -Wall
SPLIT_PGM=split_pgm.c
JOIN_PGM=join_pgm.c
SPLIT_JOIN_HEADER=split_join_header.c
LIBS=
all: split_pgm join_pgm

split_pgm:	$(SPLIT_PGM) 
	$(CC) $(CFLAGS) $(SPLIT_PGM) $(SPLIT_JOIN_HEADER) -o split_pgm $(LIBS)

join_pgm:	$(JOIN_PGM) 
	$(CC) $(CFLAGS) $(JOIN_PGM) $(SPLIT_JOIN_HEADER) -o join_pgm $(LIBS)
clean: 
	rm -f split_pgm join_pgm
