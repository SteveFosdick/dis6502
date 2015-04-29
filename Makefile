OBJS = main.o initopts.o lex.o ref.o print.o tbl.o
SRCS = dis.h main.c initopts.c lex.l ref.c print.c tbl.c 
CFLAGS = +p -n  -DAMIGA


dis:		$(OBJS)
		ln -g $(OBJS) -o dis -lcl32

tbl.o:		dis.h tbl.c

initopts.o:	dis.h initopts.c

main.o:		dis.h main.c

lex.c:		lex.l

lex.o:		lex.c dis.h

ref.o:		dis.h ref.c

print.o:	dis.h print.c

