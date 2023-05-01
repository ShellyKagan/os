#ifndef _READY_QUEUE_H_
#define _READY_QUEUE_H_

#include <stdio.h>
#include "uthreads.h"
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

int ready_queue[MAX_THREAD_NUM + 1];


/**
 * inserts an item to the queue
 * @param value an int which will be pushed to the queue
 * @return the pushed value on success, -1 on fail
 */
int push_to_ready(int);

/**
 * removes an item from the queue and returns it
 * @param value an int which will be popped from the queue
 * @return the item on success, -1 on fail
 */
int pop_from_ready();

/**
 * removes the thread with the givven id from the ready queue
 * @param tid the id of the thread which will be removed
 * @return 0 on success, -1 on failure (item doesnt exists)
 */
int remove_from_ready(int tid);

void ready_queue_display();

void move_objects_backward(int start_index);

#endif //_READY_QUEUE_H_
