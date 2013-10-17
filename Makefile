CFLAGS= -g -I/usr/local/include
LIBS= 
.SUFFIXES:
.SUFFIXES:	.o .o .c

.c.o:
	$(CC) $(CFLAGS) -c $<


OBJ=	smc386c.o 

scc: $(OBJ) 
	$(CC) -g -o c0 $(OBJ)  $(LIBS)

clean:
	rm -f *.o
	rm -f *.s
