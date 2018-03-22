CC = gcc -Wall
LIB_NAME = library

static:
	$(CC) -c $(LIB_NAME).c -o $(LIB_NAME).o
	ar rcs $(LIB_NAME).a $(LIB_NAME).o
	$(CC) -o test main.c $(LIB_NAME).a
	make tests

shared:
	$(CC) -fPIC -c $(LIB_NAME).c -o $(LIB_NAME).o
	$(CC) -fPIC -shared -o $(LIB_NAME).so $(LIB_NAME).o
	make tests

tests:
	./test 30000 100 dynamic
	./test 30000 200 dynamic create replace 3 10 find 4
	./test 30000 200 dynamic create replace 3 10 replace_alt 5 10

	./test 30 10 static
	./test 30 20 static create replace 3 10 find 4
	./test 30 20 static create replace 3 10 replace_alt 5 10

clean:
	rm -f *~
	rm -f *.o
	rm -f *.a
	rm -f *.so
