cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -pthread -lrt
objs =
main: main.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) main
