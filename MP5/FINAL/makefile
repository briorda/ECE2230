# 
# Austin Johnson
# agjohns
# 
# The makefile for MP5.
#
# make          -- to build program lab2

comp = gcc
comp_flags = -g -Wall -std=gnu99
comp_libs = -lm  

lab5 : bst.o lab5.o
	$(comp) $(comp_flags) -Wall -g bst.o lab5.o -o lab5 $(comp_libs)

bst.o : bst.c bst.h
	$(comp) $(comp_flags) -Wall -g -c bst.c

lab5.o : lab5.c bst.h 
	$(comp) $(comp_flags) -Wall -g -c lab5.c

clean :
	rm -f *.o lab2 testlist core a.out

