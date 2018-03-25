# The makefile for MP2.
#
# make          -- to build program lab2
# make design   -- to verify no illegal variables
# make testlist -- to compile testing program
# make clean    -- to delete object files, executable, and core
# make list.o   -- to compile only list.o
#
# You should not need to change this file.  
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line

lab2 : list.o soc_support.o lab2.o
	gcc -Wall -g list.o soc_support.o lab2.o -o lab2

list.o : list.c datatypes.h list.h
	gcc -Wall -g -c list.c

soc_support.o : soc_support.c datatypes.h list.h soc_support.h
	gcc -Wall -g -c soc_support.c

lab2.o : lab2.c datatypes.h list.h soc_support.h
	gcc -Wall -g -c lab2.c

testlist : testlist.o list.o soc_support.o
	gcc -Wall -g list.o soc_support.o testlist.o -o testlist

testlist.o : testlist.c datatypes.h list.h soc_support.h
	gcc -Wall -g -c testlist.c

#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
design :
	@grep -e "-> *ll_head" lab2.c soc_support.c ||:
	@grep -e "-> *ll_tail" lab2.c soc_support.c ||:
	@grep -e "-> *ll_entries" lab2.c soc_support.c ||:
	@grep -e "-> *ll_sorted_state" lab2.c soc_support.c ||:
	@grep -e "-> *ll_next" lab2.c soc_support.c ||:
	@grep -e "-> *ll_prev" lab2.c soc_support.c ||:
	@grep -e "-> *data_ptr" lab2.c soc_support.c ||:
	@grep "comp_proc" lab2.c soc_support.c ||:
	@grep "list_node_t" lab2.c soc_support.c ||:
	@grep "socket_id" list.c ||:
	@grep "dest_ip_addr" list.c ||:
	@grep "socket_t" list.c ||:
	@grep "soc_" list.c ||:

clean :
	rm -f *.o lab2 testlist core a.out

