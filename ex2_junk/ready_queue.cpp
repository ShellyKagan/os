// Queue implementation in C
#include "ready_queue.h"
int front = -1;
int rear = -1;
/**
 * inserts an item to the queue
 * @param value an int which will be pushed to the queue
 * @return the pushed value on success, -1 on fail
 */
int push_to_ready(int value) {
  if (rear == MAX_THREAD_NUM - 1)
    return -1;
  else {
    if (front == -1)
      front = 0;
    rear++;
    ready_queue[rear] = value;
    return value;
  }
}

/**
 * removes an item from the queue and returns it
 * @param value an int which will be popped from the queue
 * @return the item on success, -1 on fail
 */
int pop_from_ready() {
  if (front == -1)
    return -1;
  else {
    int item = ready_queue[front];
    front++;
    if (front > rear)
      front = rear = -1;
    return item;
  }
}

void move_objects_backward(int start_index){
  for (int i = max(start_index, 1); (i <= rear) & (i < MAX_THREAD_NUM); i++){
    ready_queue[i-1] = ready_queue[i];
  }
  rear--;
}

/**
 * removes the thread with the givven id from the ready queue
 * @param tid the id of the thread which will be removed
 * @return 0 on success, -1 on failure (item doesnt exists)
 */
int remove_from_ready(int tid){
  for (int i = front; i <= rear; i++){
    if (tid == ready_queue[i]){
      move_objects_backward (i + 1);
      return 0;
    }
  }
  return -1;
}

// Function to print the queue - for debugging purpose
void ready_queue_display()
{
  if (rear == -1)
  {
    printf ("\nQueue is Empty!!!");
    fflush (stdout);
  }
  else {
    int i;
    printf("\nQueue elements are:\n");
    fflush(stdout);
    for (i = front; i <= rear; i++)
    {
      printf("%d  ", ready_queue[i]);
      fflush(stdout);
    }
  }
  printf("\n");
  fflush(stdout);
}

