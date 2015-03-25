cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs = ryglowanie.o
ryglowanie: ryglowanie.o
	$(cc) $(cflags) -o $@ $^
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a ftw recursive
