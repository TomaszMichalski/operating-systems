filepath='..'
op='>'
date='2018 Mar 10 02:29:00'

all: static tests clean

static:
	gcc -c lib.c -o lib.o
	gcc -o main main.c lib.o

tests:
	./main $(filepath) $(op) $(date)

clean:
	rm -f *.o
	rm -f *~
