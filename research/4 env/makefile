cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic
lflags = 
objs = main.o
main: $(objs)
	$(cxx) -o $@ $^
$(objs): %.o: %.c
	$(cc) $(cflags)  -c -pedantic $<	
clean:
	rm -f *.o
