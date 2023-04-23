#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "uthreads.h"
#include "ready_queue.c"
#include "sleeping_list.c"
#include <signal.h>
#define FAIL -1
#define SUCCESS 0

typedef enum STATE
{
    RUN, READY, BLOCKED, NOTEXISTS
} STATE;
typedef void (*sig_handler) (int);

typedef struct Thread
{
    int id;
    STATE state;
    char *stack;
    thread_entry_point entry_point;
    int quantums;
    int remain_sleep_time; // in quantums
} Thread;

Thread threads[MAX_THREAD_NUM];
int running_process_id = 0;
int current_threads_amount = 0;
int total_tick = 0;
int quantum_len = 0;


// ----------------------- LOCAL FUNCTIONS -----------------------------
// todo: I have written it late in night, we should check it carefully!
// todo: add increasment of total_tick when we start a new thread as a
//  result of blocking
/**
 * moves the current process to "current_new_state" and activates the first
 * thread in the ready queue.
 * @param current_new_state the state to which the current state will be
 * transfered
 * @return 0 on success, -1 on fail
 */
int schedule (STATE current_new_state)
{
  threads[running_process_id].state = current_new_state; // is't it an array
  // of Threads? It should be "threads[running_process_id].state=..."
  if (current_new_state == READY &
      threads[running_process_id].remain_sleep_time <= 0)
  {
    push_to_ready (running_process_id);
  }
  if (current_new_state != RUN) // isn't it supposed to be ==?
  {
    running_process_id = pop_from_ready();
    if (running_process_id == FAIL)
    {
      printf ("thread library error: there are no threads to run\n");
      fflush (stderr);
      return FAIL;
    }
    threads[running_process_id].state = RUN;
  }
  return SUCCESS;
  // what happend if the proccess is sleeping? shouldn't we do something
  // eiterway?
}

/**
 * sets a timer according to the given arguments.
 * @param timer_handler a function which will be activated once the clock ticks
 * @param value the initial period for the clock
 * @param interval the frequency in which the clock will raise sig
 * @return 0 on success, -1 otherwise
 */
int set_clock (sig_handler timer_handler, int value, int interval)
{
  struct sigaction sa = {0};
  struct itimerval timer;

  sa.sa_handler = timer_handler;
  if (sigaction (SIGVTALRM, &sa, NULL) < 0)
  {
    printf ("system error: sigaction error.\n");
    fflush (stderr);
    exit (1);
    return -1;
  }

  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = value;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = interval;

  if (setitimer (ITIMER_VIRTUAL, &timer, NULL))
  {
    printf ("system error: setitimer error.\n");
    fflush (stderr);
    exit (1);
    return -1;
  }
  return SUCCESS;
}

/**
 * finds the first free space in the threads array.
 * @return the id which was found on success, -1 otherwise.
 */
int look_for_id ()
{
  for (int i = 1; i < MAX_THREAD_NUM; i++)
  {
    if (threads[i].state == NOTEXISTS)
    {  // sign for empty
      return i;
    }
  }
  return -1;
}

/**
 * checks if the thread exists. if not - output the relevant output and
 * returns -1. otherwise returns 0.
 * @param tid the id of the thread
 */
int is_exists (int tid)
{
  if (threads[tid].state == NOTEXISTS)
  {
    printf ("thread library error: thread does`nt exists\n");
    fflush (stderr);
    return FAIL;
  }
  return SUCCESS;
}

// todo: I have written it late in night, we should check it carefully!
void manage_sleepers ()
{
  struct node *ptr;
  ptr = head;
  int sleepers_to_delete[get_sleepers_amount()];
  int last_delete_index = -1;
  while (ptr != NULL)
  {
    int sleeper_id = ptr->data;
    if (--threads[sleeper_id].remain_sleep_time <= 0)
    {
      if (threads[sleeper_id].state != BLOCKED){
        threads[sleeper_id].state = READY;
        push_to_ready (sleeper_id);
      }
      sleepers_to_delete[last_delete_index++] = sleeper_id;
    }
    ptr = ptr->next;
  }
  while (last_delete_index > 0){  // delete the awake threads from the list
    delete_fron_sleeping (sleepers_to_delete[last_delete_index--]);
  }
}

void on_tick (int sig)
{
  threads[running_process_id].quantums++;
  manage_sleepers ();
  schedule (READY);
  total_tick++;
}

// ----------------------- API FUNCTIONS -----------------------------

/**
 * @brief initializes the thread library.
 *
 * Once this function returns, the main thread (tid == 0) will be set as RUNNING. There is no need to
 * provide an entry_point or to create a stack for the main thread - it will be using the "regular" stack and PC.
 * You may assume that this function is called before any other thread library function, and that it is called
 * exactly once.
 * The input to the function is the length of a quantum in micro-seconds.
 * It is an error to call this function with non-positive quantum_usecs.
 *
 * @return On success, return 0. On failure, return -1.
*/
// todo: understand what to do with the first one
// todo: figure out what are the initialization values
// todo: finish writing
int uthread_init (int quantum_usecs)
{
  set_clock(on_tick, quantum_usecs, quantum_usecs);
  // init threads array
  Thread default_thread = {0, NOTEXISTS, NULL, NULL, 1};
  for(int id = 0; id < MAX_THREAD_NUM; id++){
    threads[id] = default_thread;
  }
  return SUCCESS;
}

/**
 * @brief Creates a new thread, whose entry point is the function entry_point with the signature
 * void entry_point(void).
 *
 * The thread is added to the end of the READY threads list.
 * The uthread_spawn function should fail if it would cause the number of concurrent threads to exceed the
 * limit (MAX_THREAD_NUM).
 * Each thread should be allocated with a stack of size STACK_SIZE bytes.
 * It is an error to call this function with a null entry_point.
 *
 * @return On success, return the ID of the created thread. On failure, return -1.
*/
int uthread_spawn (thread_entry_point entry_point)
{
  // initialization & error checking
  if (current_threads_amount > MAX_THREAD_NUM || entry_point == NULL)
  {
    return FAIL;
  }
  int id = look_for_id ();
  if (id == FAIL)
  {
    return FAIL;
  }
  char *stack = malloc (sizeof (stack) * STACK_SIZE);
  if (stack == NULL)
  {
    return FAIL;
  }

  // create the new thread and pushes it to the ready queue
  Thread new_thread = {id, READY, stack, entry_point, 1};
  threads[id] = new_thread;
  current_threads_amount++;
  return push_to_ready (id);
}

// todo: write
/**
 * @brief Terminates the thread with ID tid and deletes it from all relevant control structures.
 *
 * All the resources allocated by the library for this thread should be released. If no thread with ID tid exists it
 * is considered an error. Terminating the main thread (tid == 0) will result in the termination of the entire
 * process using exit(0) (after releasing the assigned library memory).
 *
 * @return The function returns 0 if the thread was successfully terminated and -1 otherwise. If a thread terminates
 * itself or the main thread is terminated, the function does not return.
*/
int uthread_terminate (int tid)
{
  if (is_exists (tid) == FAIL){
    return FAIL;
  }

  // delete from ready queue
  if (threads[tid].state == READY){
    remove_from_ready(tid);
  }

  // delete from sleeping list
  if (threads[tid].remain_sleep_time > 0){
    delete_fron_sleeping(tid);
  }

  //delete from threads array
  free(threads[tid].stack);
  threads[tid].state = NOTEXISTS;
  current_threads_amount--;

  if (tid == 0)
  {
    exit(0);
  }
  return SUCCESS;
}

/**
 * @brief Blocks the thread with ID tid. The thread may be resumed later using uthread_resume.
 *
 * If no thread with ID tid exists it is considered as an error. In addition, it is an error to try blocking the
 * main thread (tid == 0). If a thread blocks itself, a scheduling decision should be made. Blocking a thread in
 * BLOCKED state has no effect and is not considered an error.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_block (int tid)
{
  if (tid == 0)
  {
    printf ("thread library error: cant block the main thread\n");
    fflush (stderr);
    return FAIL;
  }
  if (is_exists (tid) == FAIL)
  {
    return FAIL;
  }
  if (tid == running_process_id)
  {
    return schedule (BLOCKED);
  }
  else if (threads[tid].state == READY)
  {
    if (remove_from_ready (tid) == FAIL)
    {
      return FAIL;
    }
    threads[tid].state = BLOCKED;
  }
  return SUCCESS;
}

/**
 * @brief Resumes a blocked thread with ID tid and moves it to the READY state.
 *
 * Resuming a thread in a RUNNING or READY state has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered an error.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_resume (int tid)
{
  if (is_exists (tid) == FAIL)
  {
    return FAIL;
  }
  if (threads[tid].state == BLOCKED)
  {
    if (threads[tid].remain_sleep_time <= 0)
    {
      threads[tid].state = READY;
      push_to_ready (tid);
    }
    else
    {
      threads[tid].state = RUN; // when it will wake up it will return to ready
    }
  }
  return SUCCESS;
}

/**
 * @brief Blocks the RUNNING thread for num_quantums quantums.
 *
 * Immediately after the RUNNING thread transitions to the BLOCKED state a scheduling decision should be made.
 * After the sleeping time is over, the thread should go back to the end of the READY queue.
 * If the thread which was just RUNNING should also be added to the READY queue, or if multiple threads wake up
 * at the same time, the order in which they're added to the end of the READY queue doesn't matter.
 * The number of quantums refers to the number of times a new quantum starts, regardless of the reason. Specifically,
 * the quantum of the thread which has made the call to uthread_sleep isn’t counted.
 * It is considered an error if the main thread (tid == 0) calls this function.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_sleep (int num_quantums)
{
  if (num_quantums <= 0)
  { // todo: needs to be positive or not-negative?
    printf ("thread library error: num_quantums should be positive\n");
    fflush (stderr);
    return FAIL;
  }
  // todo: make sure it should be +1 (since the current doesnt count)
  threads[running_process_id].remain_sleep_time = num_quantums + 1;
  if (insert_to_sleeping (running_process_id) == SYSTEM_ERROR)
  {
    printf ("system error: memory error.\n");
    fflush (stderr);
    exit (1);
  }
  return schedule (RUN);
}

/**
 * @brief Returns the thread ID of the calling thread.
 *
 * @return The ID of the calling thread.
*/
int uthread_get_tid ()
{
  return running_process_id;
}

/**
 * @brief Returns the total number of quantums since the library was initialized, including the current quantum.
 *
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number should be increased by 1.
 *
 * @return The total number of quantums.
*/
int uthread_get_total_quantums ()
{
  // todo: what does it means "including the current"?
  return total_tick;
}

/**
 * @brief Returns the number of quantums the thread with ID tid was in RUNNING state.
 *
 * On the first time a thread runs, the function should return 1. Every additional quantum that the thread starts should
 * increase this value by 1 (so if the thread with ID tid is in RUNNING state when this function is called, include
 * also the current quantum). If no thread with ID tid exists it is considered an error.
 *
 * @return On success, return the number of quantums of the thread with ID tid. On failure, return -1.
*/
int uthread_get_quantums (int tid)
{
  if (is_exists (tid) == FAIL)
  {
    return FAIL;
  }
  return threads[tid].quantums;
}