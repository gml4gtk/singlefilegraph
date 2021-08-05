
SRC = \
	main.c \
	d4dag.c

all:
	make clean
	make indent
	gcc -Wall -Wextra -pedantic -O0 main.c d4dag.c -o main
	rm -v -f *.o
	g++ -Wall -Wextra -pedantic -O0 main.c d4dag.c -o main++
	rm -v -f *~

indent:
	./lindent.sh $(SRC)

clean:
	rm -v -f main
	rm -v -f main++
	rm -v -f *.o
	rm -v -f *.output
	rm -v -f *~

cleaner:
	make clean

# end.
