CC= gcc -std=c99
CFLAGS= -no-pie -Wall -Wextra -g

taco:	test.o da.o rbt.o dictionary.o fileio.o tnode.o bst.o gst.o cda.o queue.o boggle.o die.o grid.o -lncurses -lm
	$(CC) $(CFLAGS) test.o da.o rbt.o dictionary.o fileio.o tnode.o bst.o gst.o cda.o queue.o boggle.o die.o grid.o -o test -lncurses -lm
test: taco 
	./test 
valgrind: taco 
	valgrind --leak-check=full ./test
taco1: testgrid.o da.o grid.o player.o die.o boggle.o stack.o dictionary.o rbt.o gst.o bst.o tnode.o queue.o cda.o fileio.o -lncurses -lm
	$(CC) $(CFLAGS) testgrid.o da.o grid.o player.o die.o boggle.o stack.o dictionary.o rbt.o gst.o bst.o tnode.o queue.o cda.o fileio.o -o testgrid -lncurses -lm
test1: taco1
	./testgrid
valgrind1: taco1
	valgrind --leak-check=full ./testgrid
test.o: test.c da.h rbt.h dictionary.h fileio.h
	$(CC) $(CFLAGS) -c test.c
da.o: da.c da.h
	$(CC) $(CFLAGS) -c da.c
cda.o: cda.c cda.h
	$(CC) $(CFLAGS) -c cda.c
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c
tnode.o: tnode.c tnode.h
	$(CC) $(CFLAGS) -c tnode.c
bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c
gst.o: gst.c gst.h
	$(CC) $(CLFAGS) -c gst.c
rbt.o: rbt.c rbt.h
	$(CC) $(CFLAGS) -c rbt.c
dictionary.o: dictionary.c dictionary.h
	$(CC) $(CFLAGS) -c dictionary.c
fileio.o: fileio.c fileio.h
	$(CC) $(CFLAGS) -c fileio.c
testgrid.o: testgrid.c
	$(CC) $(CFLAGS) -c testgrid.c
grid.o: grid.c grid.h
	$(CC) $(CFLAGS) -c grid.c
player.o: player.c player.h
	$(CC) $(CFLAGS) -c player.c
die.o: die.c die.h
	$(CC) $(CFLAGS) -c die.c 
boggle.o: boggle.c boggle.h
	$(CC) $(CFLAGS) -c boggle.c
stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c
clean: 
	$(RM) *.exe *.o	main *~
