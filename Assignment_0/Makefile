#this target will compile all the file
all: The_Knights_Tour

The_Knights_Tour: The_Knights_Tour.o
	gcc The_Knights_Tour.c -o The_Knights_Tour

The_Knights_Tour.o: The_Knights_Tour.c
	gcc -c The_Knights_Tour.c

clean:
	rm -rf *o The_Knights_Tour
