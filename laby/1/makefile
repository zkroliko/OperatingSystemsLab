cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall 
lflags = -Wall 
objs = main.o list.o
main: $(objs)
	$(cc) $(cflags) -o $@ $^
$(objs): %.o: %.c
	$(cc) $(cflags)  -c -pedantic $<	
clean:
	rm -f *.o
