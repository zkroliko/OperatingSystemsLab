cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -pthread -lrt
objs =
MAIN: client server
	
client: client.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
server: server.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) main
