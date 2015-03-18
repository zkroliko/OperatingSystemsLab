# Defining some color for echo
red='\033[0;31m'
NC='\033[0m' # No Color
# Some variables
cc = gcc
cxx = g++
cflags = -std=gnu99 -Wall -g
lflags = -Wall 
objs = main.o mainDynamic.o
# Targets
all: static shared dynamic
	# Making all targets
static: main.o ../zad1/liblist.a
	 $(cc) $(cflags) -o $@ -L../zad1/ $^ -llist
shared: main.o ../zad1/liblist.so
	$(cc) $(cflags) -o $@ -L../zad1/ -I . -Wl,-rpath=../zad1 $^  -llist
dynamic: mainDynamic.o
	$(cc) $(cflags) -o $@ -L../zad1/ -I . $^ -ldl
# Targets for object files and error messages
../zad1/liblist.a:
	@echo "${red}Please compile the static library first, in the zad1 folder.${NC}"
../zad1/liblist.so:
	@echo "${red}Please compile the dynamic first, in the zad1 folder.${NC}"
%.o: %.c 
	$(cc) $(cflags) -shared -fPIC -c $< -ldl 
clean:
	rm -f *.o *.a $(objs) static shared dynamic
