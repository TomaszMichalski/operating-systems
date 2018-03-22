CC = gcc -Wall
LIB_NAME = library

all: static shared

static:
	$(CC) -c $(LIB_NAME).c -o $(LIB_NAME).o
	ar rcs $(LIB_NAME).a $(LIB_NAME).o

shared:
	$(CC) -fPIC -c $(LIB_NAME).c -o $(LIB_NAME).o
	$(CC) -fPIC -shared -o $(LIB_NAME).so $(LIB_NAME).o

clean:
	rm -f *~
	rm -f *.o
	rm -f *.a
	rm -f *.so
