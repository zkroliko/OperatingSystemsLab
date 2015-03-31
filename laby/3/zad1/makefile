cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs =
main: main.o functions.o
	$(cc) $(cflags) -o $@ $^
%.o: %.c 
	$(cc) $(cflags) -c -pedantic $<	
clean:
	rm -f *.o *.a $(objs) main
