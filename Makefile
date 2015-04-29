OBJS = main.o initopts.o lex.o ref.o print.o tbl.o
SRCS = dis.h main.c initopts.c lex.l ref.c print.c tbl.c 
CFLAGS = -O

dis:		$(OBJS)
		cc $(OBJS) -o dis

tbl.o:		dis.h tbl.c
		cc -c tbl.c

initopts.o:	dis.h initopts.c

main.o:		dis.h main.c

lex.o:		lex.c

lex.c:		dis.h lex.l

ref.o:		dis.h ref.c

print.o:	dis.h print.c

dis.man:	dis.1
		nroff -man dis.1 > dis.man

install:	dis
		cp dis /a/rgb/bin/dis6502

clean:
		rm -f $(OBJS) lex.c dis.man

clobber:	clean
		rm -f dis

ckpt:		$(SRCS)
		ci -l $(SRCS)

lint: dis.h main.c initopts.c lex.c ref.c print.c tbl.c 
		lint  dis.h main.c initopts.c lex.c ref.c print.c tbl.c 

shar:		Makefile dis.1 $(SRCS)
		shar -f shar Makefile dis.1 $(SRCS)
