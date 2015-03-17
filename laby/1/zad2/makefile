cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs = main.o ../zad1/list.o
static: main.o liblist.a
	$(cc) $(cflags) -static -L. -llist -o $@ $^	
shared:
dynamic:
liblist.a: $(objs)
	ar rcs $@ list.o
../zad1/%.o: %.c 
	$(cc) $(cflags) -c -pedantic $<	
clean:
	rm -f *.o *.a $(objs) test
