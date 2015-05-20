cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs = main
main: main.o
	$(cc) $(cflags) -o $@ $^ -lpthread 
%.o: %.c 
	$(cc) $(cflags) -c -pedantic $<	
clean:
	rm -f *.o *.a $(objs) main
