cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs = main.o geneator.o
main: test generator sortSys sortLib
	
test: main.o generator.o functions.o
	$(cc) $(cflags) -o $@ $^
generator: mainGenerator.o generator.o
	$(cc) $(cflags) -o $@ $^
sortSys: mainSortSys.o functions.o
	$(cc) $(cflags) -o $@ $^
sortLib: mainSortLib.o functions.o
	$(cc) $(cflags) -o $@ $^
%.o: %.c 
	$(cc) $(cflags) -c -pedantic $<	
clean:
	rm -f *.o *.a $(objs) test main generator sortLib sortSys rekordy/*
