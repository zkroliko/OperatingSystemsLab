cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs = main.o list.o
test: main.o liblist.a
	$(cc) $(cflags) -static -L. -llist -o $@ $^	
liblist.a: $(objs)
	ar rcs $@ list.o
%.o: %.c 
	$(cc) $(cflags) -c -pedantic $<	
clean:
	rm -f *.o *.a $(objs) test	
