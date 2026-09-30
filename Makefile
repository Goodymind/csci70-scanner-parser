token.o: token.c token.h
	gcc -c token.c

scanner.o: scanner.c token.h token.c
	gcc -c scanner.c

parser.o: parser.c token.h parser.h token.c scanner.o
	gcc -c parser.c

scan_tester.o: scan_tester.c
	gcc -c scan_tester.c

parse_tester.o: parse_tester.c
	gcc -c parse_tester.c

scan_tester.exe: scan_tester.o scanner.o token.o
	gcc -o scan_tester.exe scan_tester.o scanner.o token.o -fsanitize=leak,address,undefined

parse_tester.exe: parse_tester.o scanner.o token.o parser.o
	gcc -o parse_tester.exe parse_tester.o scanner.o token.o parser.o -fsanitize=leak,address,undefined

test: sample/sample1-quad-formula.txt scan_tester.exe
	./scan_tester.exe

test_parse: parse_tester.exe
	./parse_tester.exe

clean:
	rm *.exe *.o *output.txt