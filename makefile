CC= gcc -std=c99
CFLAGS= -no-pie -Wall -Wextra -g

taco:	test.o da.o -lncurses -lm
	$(CC) $(CFLAGS) test.o da.o -o test -lncurses -lm
test: taco 
	./test 
valgrind: taco 
	valgrind --leak-check=full ./test
test.o: test.c da.h 
	$(CC) $(CFLAGS) -c test.c
da.o: da.c da.h
	$(CC) $(CFLAGS) -c da.c
clean: 
	$(RM) *.exe *.o	main
