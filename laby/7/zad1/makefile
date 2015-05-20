cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs =
MAIN: main initialize dispose
	
main: main.o
	$(cc) $(cflags) -o $@ $^
initialize: initialize.o
	$(cc) $(cflags) -o $@ $^
dispose: clean.o
	$(cc) $(cflags) -o $@ $^
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) main dispose initialize
