CC=gcc
CFLAGS=-g -Wall
ADAPTIVE_RERANK=adaptive_re-rank.c
ADAPTIVE_DERANK=adaptive_de-rank.c
SETUP_SWAP=setup_swap.c
LIBS=
all: adaptive_re-rank adaptive_de-rank

adaptive_re-rank:	$(ADAPTIVE_RERANK) 
	$(CC) $(CFLAGS) $(ADAPTIVE_RERANK) $(SETUP_SWAP) -o adaptive_re-rank $(LIBS)

adaptive_de-rank:	$(ADAPTIVE_DERANK) 
	$(CC) $(CFLAGS) $(ADAPTIVE_DERANK) $(SETUP_SWAP) -o adaptive_de-rank $(LIBS)
clean: 
	rm -f adaptive_re-rank adaptive_de-rank
