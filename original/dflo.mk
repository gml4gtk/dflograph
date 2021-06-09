
# The source code in this file is hereby placed in the public domain.
# 29 August 1996, Michael Wolfe, Oregon, USA */

dflo: alloc.o dg.o dgdrive.o dgplot.o dgpost.o gg.o ggmain.o ggy.o handler.o args.o
	$(CC) -o dflo alloc.o dg.o dgdrive.o dgplot.o dgpost.o gg.o ggmain.o ggy.o handler.o args.o \
	$(LIBRARY) -lXm -lXt -lX11 -lm

gdflo: alloc.o dg.o dgdrive.o dgplot.o dgpost.o gg.o ggmain.o gdy.o handler.o args.o
	$(CC) -o gdflo alloc.o dg.o dgdrive.o dgplot.o dgpost.o gg.o ggmain.o gdy.o handler.o args.o \
	$(LIBRARY) -lXm -lXt -lX11 -lm

args.c:	$(SRC)/args.l $(SRC)/args.h
	flex -oargs.c $(SRC)/args.l 

args.o: args.c $(SRC)/args.h
	$(CC) -c -x c++ args.c

alloc.o:	$(SRC)/alloc.cc   $(SRC)/portable.h $(SRC)/standard.h 	\
		$(SRC)/error.h $(SRC)/alloc.h
	$(CC) -c $(SRC)/alloc.cc

dg.o:		$(SRC)/dg.cc	   $(SRC)/portable.h $(SRC)/standard.h \
		$(SRC)/error.h $(SRC)/alloc.h $(SRC)/dg.h $(SRC)/list.h
	$(CC) -c $(SRC)/dg.cc

dgdrive.o:	$(SRC)/dgdrive.cc $(SRC)/portable.h $(SRC)/standard.h \
		$(SRC)/error.h $(SRC)/dg.h
	$(CC) -c $(SRC)/dgdrive.cc

dgplot.o:	$(SRC)/dgplot.cc  $(SRC)/portable.h $(SRC)/standard.h \
		$(SRC)/dg.h $(SRC)/dgplot.h
	$(CC) -c $(SRC)/dgplot.cc

dgpost.o:	$(SRC)/dgpost.cc  $(SRC)/portable.h $(SRC)/standard.h \
		$(SRC)/dg.h $(SRC)/dgpost.h
	$(CC) -c $(SRC)/dgpost.cc

gg.o:		$(SRC)/gg.cc	   $(SRC)/portable.h $(SRC)/standard.h \
		$(SRC)/error.h $(SRC)/handler.h $(SRC)/dg.h $(SRC)/dgplot.h \
		$(SRC)/gg.h
	$(CC) -c $(SRC)/gg.cc

ggmain.o:	$(SRC)/ggmain.cc  $(SRC)/portable.h $(SRC)/standard.h \
		$(SRC)/error.h $(SRC)/handler.h $(SRC)/dg.h $(SRC)/dgplot.h \
		$(SRC)/dgplot.h $(SRC)/gg.h
	$(CC) -c $(SRC)/ggmain.cc

ggl.c:	$(SRC)/ggl.l
	flex -oggl.c $(SRC)/ggl.l

ggy.c:	$(SRC)/ggy.y
	bison -o ggy.c -t $(SRC)/ggy.y

ggy.o:	ggy.c ggl.c $(SRC)/gg.h
	$(CC) -c -x c++ ggy.c

gdl.c:	$(SRC)/gdl.l
	flex -ogdl.c $(SRC)/gdl.l

gdy.c:	$(SRC)/gdy.y
	bison -o gdy.c -t $(SRC)/gdy.y

gdy.o:	gdy.c gdl.c $(SRC)/gg.h
	$(CC) -c -x c++ gdy.c

handler.o:	$(SRC)/handler.cc $(SRC)/portable.h $(SRC)/error.h \
		$(SRC)/handler.h
	$(CC) -c $(SRC)/handler.cc

test:	dflo
	dflo $(SRC)/ex1.in

clean:
	touch junk.o junk.c
	rm -f dflo *.o *.c

install:
	cp dflo $(INSTALL)

dflo.tar:
	tar cvf $@ $(SRC)/*.h $(SRC)/*.cc $(SRC)/*.l $(SRC)/*.y $(SRC)/*.in $(SRC)/*.sys $(SRC)/*.mk
