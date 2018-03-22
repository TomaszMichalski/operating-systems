filename=plik
records=20
size=4

all: clean static tests

static:
	gcc -Wall -c lib.c -o lib.o
	gcc -Wall -o main main.c lib.o

tests:
	./main generate $(filename).txt $(records) $(size)
	./main copy $(filename).txt $(filename)lib.txt $(records) $(size) lib
	./main copy $(filename).txt $(filename)sys.txt $(records) $(size) sys
	./main sort $(filename)lib.txt $(records) $(size) lib
	./main sort $(filename).txt $(records) $(size) sys

clean:
	rm -f *~
	rm -f *.o
	rm -f $(filename).txt
	rm -f $(filename)lib.txt
	rm -f $(filename)sys.txt
