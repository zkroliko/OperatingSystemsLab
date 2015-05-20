cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -lrt -pthread
objs =
MAIN: main initialize dispose
	
main: main.o
	$(cc) $(cflags) -o $@ $^ $(lflags) 
initialize: initialize.o
	$(cc) $(cflags) -o $@ $^ $(lflags) 
dispose: clean.o
	$(cc) $(cflags) -o $@ $^ $(lflags) 
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) main dispose initialize
