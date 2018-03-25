# The makefile for MP1.

lab1 : socketlist.c lab1.c
	gcc -Wall -g socketlist.c lab1.c -o lab1

#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
design :
	@grep "vector_size" lab1.c ||:
	@grep "vector_capacity" lab1.c ||:
	@grep -e "-> *vector_ptr" lab1.c ||:
	@grep "socket_id" socketlist.c ||:
	@grep "_ip_addr" socketlist.c ||:
	@grep "time_received" socketlist.c ||:

clean :
	rm -f *.o lab1 core a.out

