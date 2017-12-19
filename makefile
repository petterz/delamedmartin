CC= gcc
CFLAGS= -g -c -Wall
VALGRINDFLAGS= valgrind --leak-check=full -v
LINKFLAGS= -g
OBJECTS= refmem.o allocate.o refcount.o

refmem.o: refmem.c refmem.h
	$(CC) $(CFLAGS) refmem.c

allocate.o: allocate.c allocate.h
	$(CC) $(CFLAGS) allocate.c

refcount.o: refcount.c refcount.h
	$(CC) $(CFLAGS) refcount.c

clean:
	rm *.o

valgrind_test: test
	$(VALGRINDFLAGS) ./test

test: 	$(OBJECTS) test.c
	$(CC) $(LINKFLAGS) -o test test.c $(OBJECTS) -lcunit

runtest: test
	./test

example: refmem.o example.c 
	 $(CC) $(LINKFLAGS) -o example example.c refmem.o

examplerun: example
	$(VALGRINDFLAGS) ./example
