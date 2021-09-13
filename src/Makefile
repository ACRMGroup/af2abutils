BINDIR  = $(HOME)/bin
SCRDIR  = $(HOME)/scripts
EXES    = pdbrmseq
SCRIPTS = makeaf2abpdbs.pl
CFLAGS  = -O3 -ansi -Wall -pedantic -Wno-stringop-truncation -I$(HOME)/include -L$(HOME)/lib
LFLAGS  = -lbiop -lgen -lm -lxml2
CC      = gcc

all : $(TARGETS)
	@echo " "
	@echo " --- BUILD COMPLETE --- "
	@echo " "

pdbrmseq : pdbrmseq.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

makeaf2abpdbs :  makeaf2abpdbs.pl
	ln -sf $< $@

install :
	mkdir -p $(BINDIR)
	cp $(EXES) $(BINDIR)
	mkdir -p $(SCRDIR)
	cp -p $(SCRIPTS) $(SCRDIR)
	(cd $(BINDIR); ln -sf $(SCRDIR)/makeaf2abpdbs.pl makeaf2abpdbs)



