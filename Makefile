OBJS=Xprogs.o init.o table.o xpt.o
CPPFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBS=-lX11 -lXt -lXaw
CFLAGS=-O3 -g

xpt: $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o xpt $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm *.o xpt

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<
