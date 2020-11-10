537make: 537make.o reader.o buildSpecGraph.o parseArgs.o
	gcc -Wall -pedantic -o 537make 537make.o reader.o buildSpecGraph.o parseArgs.o

537make.o: 537make.c reader.h buildSpecGraph.h parseArgs.h
	gcc -Wall -pedantic -c 537make.c

reader.o: reader.c reader.h buildSpecGraph.h parseArgs.h
	gcc -Wall -pedantic -c reader.c

buildSpecGraph.o: buildSpecGraph.c buildSpecGraph.h
	gcc -Wall -pedantic -c buildSpecGraph.c

parseArgs.o: parseArgs.c parseArgs.h
	gcc -Wall -pedantic -c parseArgs.c

clean:
	rm -f 537make.o reader.o buildSpecGraph.o parseArgs.o
