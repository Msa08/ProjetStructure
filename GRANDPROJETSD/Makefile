all: main jeu_de_test

main: main.o structure.o compute.o readwrite.o chiffrement.o
	gcc -Wall main.o structure.o compute.o readwrite.o chiffrement.o -o main -lm -lssl -lcrypto

jeu_de_test: jeu_de_test.o compute.o readwrite.o chiffrement.o structure.o
	gcc -Wall jeu_de_test.o structure.o compute.o readwrite.o chiffrement.o -o jeu_de_test -lm -lssl -lcrypto

main.o: main.c compute.h readwrite.h chiffrement.h structure.h struct.h
	gcc -Wall -c main.c -o main.o -lm -lssl -lcrypto

chiffrement.o: chiffrement.c chiffrement.h struct.h
	gcc -lm -Wall -c chiffrement.c -o chiffrement.o -lssl -lcrypto

struture.o: structure.c structure.h chiffrement.h struct.h
	gcc -Wall -c structure.c -o structure.o -lm -lssl -lcrypto

readwrite.o: readwrite.c struct.h compute.h readwrite.h chiffrement.h structure.h 
	gcc -Wall -c readwrite.c -o readwrite.o -lm -lssl -lcrypto

compute.o: compute.c structure.h compute.h readwrite.h chiffrement.h struct.h
	gcc -Wall -c compute.c -o compute.o -lm -lssl -lcrypto

jeu_de_test.o: jeu_de_test.c compute.h readwrite.h chiffrement.h structure.h struct.h
	gcc -Wall -c jeu_de_test.c -o jeu_de_test.o -lm -lssl -lcrypto

clean:
	rm -f *.o
	rm -f main
	rm -f jeu_de_test
