SHELL = cmd
CC = gcc
CFLAGS = -g -Wall --share
INCLUDES = -I..\lua-5.3.0
LIBS = -L..\lua-5.3.0 -llua53

all: ltimer

clean:
	del ltimer.dll
	
ltimer:
	$(CC) $(CFLAGS) -o ltimer.dll ltimer.c timer.c rbtree.c $(INCLUDES) $(LIBS)

