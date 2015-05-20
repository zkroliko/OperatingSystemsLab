cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs =
main: client server
	
server: server.o 
	$(cc) $(cflags) -o $@ $^ -lrt
client: client.o 
	$(cc) $(cflags) -o $@ $^ -lrt
%.o: %.c 
	$(cc) $(cflags) -c $<
clean:
	rm -f *.o *.a $(objs) client server
