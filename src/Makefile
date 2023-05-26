#######################################################################
#
#   Makefile for the taylor translator
#		

MANDIR = $(DESTDIR)/usr/share/man/man1/
BINDIR = $(DESTDIR)/usr/bin/
DOCDIR = $(DESTDIR)/usr/share/taylor/

INSTALLDOC = /usr/bin/install -c -m 644
INSTALLPROGRAM = /usr/bin/install -c -m 755

CP = cp
MKDIR = mkdir

all:     taylor

install: all
	$(MKDIR) -p $(MANDIR)
	$(MKDIR) -p $(DOCDIR)
	$(MKDIR) -p $(BINDIR)
	$(INSTALLPROGRAM) taylor $(BINDIR)
	$(INSTALLDOC) taylor.1.gz $(MANDIR)
	$(INSTALLDOC) taylor.pdf $(DOCDIR)
	$(INSTALLDOC) examples.tgz $(DOCDIR)

HEADER1= y.tab.h
HEADER2= Node.h NodeCode.h
LEXSRC = Lex.l
YACCSRC= parse.y
SRC    = lex.yy.c y.tab.c Node.c  Parse.c my_jet_code.c my_coef_code.c Code.c Code1.c Sum.c CodeMain.c Expr.c Poincare.c Main.c
OBJ    = $(SRC:.c=.o)

CC=gcc
LEXLIB = -lfl
OPFLAG = -g -Wall

Node.h: NodeCode.h
	@/usr/bin/touch Node.h

lex.yy.c: Lex.l $(HEADER1)
	flex -l Lex.l

y.tab.h: parse.y
	bison -y -d parse.y

y.tab.c: y.tab.h
	bison -y -v parse.y

taylor: $(OBJ)
	$(CC) -g -o $@ $(OPFLAG) $(OBJ) $(LEXLIB) -lm

clean:
	/bin/rm -rf *.o lex.yy.c y.tab.c y.tab.h y.output  *~ parse

.c.o:
	$(CC) $(OPFLAG)   -c $<

#######################################################################
