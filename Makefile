token.o: token.c token.h
	gcc -c token.c

scan.o: scan.c token.h token.c
	gcc -c scan.c

parse.o: parse.c token.h parse.h token.c scan.o
	gcc -c parse.c

scanner.o: scanner.c
	gcc -c scanner.c

parser.o: parser.c
	gcc -c parser.c

scanner.exe: scan.o scanner.o token.o
	gcc -o scanner.exe scan.o scanner.o token.o -fsanitize=leak,address,undefined

parser.exe: parse.o scanner.o token.o parser.o
	gcc -o parser.exe parse.o scan.o token.o parser.o -fsanitize=leak,address,undefined

scan: scanner.exe
	./scanner.exe

parse: parser.exe
	./parser.exe sample1-quad-formula.txt

parse-error: parser.exe
	./parser.exe sample2-just-tokens.txt

clean:
	rm *.exe *.o *output.txt