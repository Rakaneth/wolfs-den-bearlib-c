CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,obj/%.o,$(SRCS))
CFLAGS=-I. -Wall -g
LFLAGS=-L. -llua53 -lBearLibTerminal

obj/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

dev: $(OBJS)
	$(CC) -o blt $^ $(CFLAGS) $(LFLAGS)

clean:
	del blt.exe && cd obj && del *.o

