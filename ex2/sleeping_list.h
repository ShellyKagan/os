#ifndef _SLEEPING_LIST_H_
#define _SLEEPING_LIST_H_

#include<stdio.h>
#include<stdlib.h>

#define SYSTEM_ERROR -2
#define INPUT_ERROR -3

struct node
{
    int data;
    struct node *next;
};

struct node *head;

/**
 * inserts a thread to the sleeping list.
 * @return 0 on success, SYSTEM_ERROR or INPUT_ERROR on failure.
 */
int insert_to_sleeping (int tid);

/**
 * returns the index of the thread in the sleeping list
 * @return the index on success, SYSTEM_ERROR or INPUT_ERROR on failure.
 */
int index_in_sleeping (int tid);

int delete_fron_sleeping (int tid);

void sleeping_list_display ();

int get_sleepers_amount ();

#endif //_SLEEPING_LIST_H_
