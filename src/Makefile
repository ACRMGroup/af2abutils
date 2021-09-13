BINDIR  = ../bin
EXES    = pdbrmseq makeaf2abpdbs
CFLAGS  = -O3 -ansi -Wall -pedantic -Wno-stringop-truncation -I$(HOME)/include -L$(HOME)/lib
LFLAGS  = -lbiop -lgen -lm -lxml2
CC      = gcc

all : $(EXES)
	@echo " "
	@echo " --- BUILD COMPLETE --- "
	@echo " "

pdbrmseq : pdbrmseq.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

makeaf2abpdbs :  makeaf2abpdbs.pl
	ln -sf $< $@

install :
	mkdir -p $(BINDIR)
	cp -p $(EXES) $(BINDIR)
	cp *.pm $(BINDIR)

