# 
# Austin Johnson
# agjohns
# 
# The makefile for MP6.
#
# make          -- to build program lab6

comp = gcc
comp_flags = -g -Wall -std=gnu99
comp_libs = -lm  

lab6 : graph.o lab6.o
	$(comp) $(comp_flags) -Wall -g graph.o lab6.o -o lab6 $(comp_libs)

graph.o : graph.c graph.h
	$(comp) $(comp_flags) -Wall -g -c graph.c $(comp_libs)

lab6.o : lab6.c graph.h
	$(comp) $(comp_flags) -Wall -g -c lab6.c

clean :
	rm -f *.o lab2 testlist core a.out
