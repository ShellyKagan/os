# uthread_main: uthreads.h  uthreads.c sleeping_list.c ready_queue.c main.c
#	gcc -Wall -Wextra -Wvla -std=gnu99  uthreads.h uthreads.c sleeping_list.h sleeping_list.c ready_queue.h ready_queue.c main.c -o main


main: ex2/uthreads.h  uthreads.c sleeping_list.c ready_queue.c test1.in.cpp
	g++ -Wall -O -g test1.in.cpp uthreads.h uthreads.c sleeping_list.h sleeping_list.c ready_queue.h ready_queue.c -o main