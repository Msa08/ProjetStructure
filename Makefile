CC = gcc

main : main.o prime_number.o chiffrement.o key.o Base.o Hachage.o Block.o Blocktree.o simulation.o
	$(CC) -g -o main  $^ -lm -lssl -lcrypto

main.o : main.c
	$(CC) -g -o main.o -c $^

prime_number.o : prime_number.c
	$(CC) -g -o prime_number.o -c $^

chiffrement.o : chiffrement.c
	$(CC) -g -o chiffrement.o -c $^

key.o : key.c
	$(CC) -g -o key.o -c $^

Base.o : Base.c
	$(CC) -g -o Base.o -c $^

Hachage.o : hachage.c
	$(CC) -g -o Hachage.o -c $^

Block.o :block.c
	$(CC) -g -o Block.o -c $^ -lssl -lcrypto

Blocktree.o :blocktree.c
	$(CC) -g -o Blocktree.o -c $^

simulation.o :simulation.c
	$(CC) -g -o simulation.o -c $^