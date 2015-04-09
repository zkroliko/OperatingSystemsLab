cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs =
main: main.o functions.o
	$(cc) $(cflags) -o $@ $^
csv:
	$(cc) $(cflags) -DCSV -c main.c	
	$(cc) $(cflags) -DCSV -c functions.c	
	$(cc) $(cflags) -o $@ main.o functions.o 
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) main csv
