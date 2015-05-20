cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -pthread
objs =
main: generator a_wariant b_wariant c_wariant
	
a_wariant: a_wariant.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
b_wariant: b_wariant.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
c_wariant: c_wariant.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
generator: generator.o
	$(cc) $(cflags) -o $@ $^ $(lflags)
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a $(objs) a_wariant b_wariant c_wariant generator
