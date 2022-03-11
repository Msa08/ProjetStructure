CC = gcc

main : main.o prime_number.o
	$(CC) -o main $^

main.o : main.c
	$(CC) -o main.o -c $^

prime_number.o : prime_number.c
	$(CC) -o prime_number.o -c $^
