cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs =
main: wrfifo rdfifo 
	
wrfifo: wrfifo.o 
	$(cc) $(cflags) -o $@ $^
rdfifo: rdfifo.o 
	$(cc) $(cflags) -o $@ $^
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) wrfifo rdfifo
