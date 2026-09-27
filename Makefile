scanner.o: scanner.c token.h
	gcc -c scanner.c

scan_tester.o: scanner.o scan_tester.c
	gcc -c scan_tester.c

scan_tester.exe: scan_tester.o scanner.o token.h
	gcc -o scan_tester.exe scan_tester.o scanner.o

test: sample/sample1-quad-formula.txt scan_tester.exe
	./scan_tester.exe

clean:
	rm *.exe *.o