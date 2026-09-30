token.o: token.c token.h
	gcc -c token.c

scanner.o: scanner.c token.h token.c
	gcc -c scanner.c

scan_tester.o: scanner.o scan_tester.c
	gcc -c scan_tester.c

scan_tester.exe: scan_tester.o scanner.o token.o
	gcc -o scan_tester.exe scan_tester.o scanner.o token.o -fsanitize=leak,address,undefined

test: sample/sample1-quad-formula.txt scan_tester.exe
	./scan_tester.exe

clean:
	rm *.exe *.o *output.txt