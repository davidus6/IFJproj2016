BIN=project
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g 
RM=rm -f

ALL: instructions.o main.o stack.o interpret.o scanner.o str.o parser.o prec.o pstack.o semantic.o ial.o functions.o
	 $(CC) $(CFLAGS) -o $(BIN) instructions.o main.o stack.o interpret.o scanner.o str.o parser.o prec.o pstack.o semantic.o ial.o functions.o -lm

#pouze precedencni
#ALL: scanner.o prec.o pstack.o precmain.o str.o
#	$(CC) $(CFLAGS) -o $(BIN) scanner.o prec.o pstack.o precmain.o str.o -lm

clean:
	$(RM) *.o $(BIN)

test:
	bash tests/runTests.sh