cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g -fPIC
lflags = -Wall 
objs = list.o
libs: liblist.so liblist.a	
liblist.so: list.o
	$(cc) $(cflags) -shared -o liblist.so list.o
liblist.a: $(objs)
	ar -crs $@ list.o
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a *.so $(objs)
