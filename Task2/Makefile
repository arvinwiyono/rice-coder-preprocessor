CC=gcc
CFLAGS=-g -Wall
CONTEXT_RERANK=context_re-rank.c
CONTEXT_DERANK=context_de-rank.c
CONTEXT=context.c
LIBS=
all: context_re-rank context_de-rank

context_re-rank:	$(CONTEXT_RERANK) 
	$(CC) $(CFLAGS) $(CONTEXT_RERANK) $(CONTEXT) -o context_re-rank $(LIBS)

context_de-rank:	$(CONTEXT_DERANK) 
	$(CC) $(CFLAGS) $(CONTEXT_DERANK) $(CONTEXT) -o context_de-rank $(LIBS)
clean: 
	rm -f context_re-rank context_de-rank
