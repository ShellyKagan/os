#ifndef UTHREADS_C
#define UTHREADS_C

#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "uthreads.h"
#include "ready_queue.h"
#include "sleeping_list.h"


#define FAIL -1
#define SUCCESS 0


typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7



typedef enum STATE
{
    RUN, READY, BLOCKED, NOTEXISTS, SLEEPING
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
sigjmp_buf env[MAX_THREAD_NUM];

int running_process_id = 0;
int current_threads_amount = 0;
int total_tick = 0;
int quantum_len = 0;


// ----------------------- LOCAL FUNCTIONS -----------------------------
void jump_to_thread(int tid)
{
  running_process_id = tid;
  siglongjmp(env[tid], 1);
}

/**
 * @brief Saves the current thread state, and jumps to the other thread.
 */
void yield(int jump_tid)
{
  int ret_val = sigsetjmp(env[running_process_id], 1);
  printf("yield: ret_val=%d\n", ret_val);
  if (ret_val == 0)
  {
    jump_to_thread(jump_tid);
  }
}

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
  address_t ret;
  asm volatile("xor    %%fs:0x30,%0\n"
               "rol    $0x11,%0\n"
      : "=g" (ret)
      : "0" (addr));
  return ret;
}

void setup_thread(int tid, char *stack, thread_entry_point entry_point)
{
  // initializes env[tid] to use the right stack, and to run from the function 'entry_point', when we'll use
  // siglongjmp to jump into the thread.
  address_t sp = (address_t) stack + STACK_SIZE - sizeof(address_t);
  address_t pc = (address_t) entry_point;
  sigsetjmp(env[tid], 1);
  (env[tid]->__jmpbuf)[JB_SP] = translate_address(sp);
  (env[tid]->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&env[tid]->__saved_mask);
}

void manage_sleepers ();

/**
 * Display all relevant data structures status for debugging
 */
void display_status(){
  printf ("---------------THREADS ARRAY------------------\n");
  fflush(stdout);
  for(int i = 0; i<MAX_THREAD_NUM; i++){
    printf ("thread id: %d, state: %u, \n", i, threads[i].state);
    fflush(stdout);
  }
  printf ("-----------------READY QUEUE----------------\n");
  fflush(stdout);
  ready_queue_display();
  printf ("---------------SLEEPING LIST------------------\n");
  fflush(stdout);
  sleeping_list_display();
}

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
 ///todo: if ready = -1, return error
int schedule (STATE current_new_state)
{
  printf("ASASAS entered function schedule. running is %d. parameter: %d\n",
         running_process_id, current_new_state);
  fflush (stdout);

  threads[running_process_id].state = current_new_state;
  if (current_new_state == READY)
  {
    push_to_ready (running_process_id);
  }
  if (current_new_state != RUN)
  {
    running_process_id = pop_from_ready ();
    if (running_process_id == FAIL)
    {
      printf ("thread library error: there are no threads to run\n");
      fflush (stderr);
      return FAIL;
    }
    threads[running_process_id].state = RUN;
    jump_to_thread (running_process_id);
  }
  fflush (stdout);
  return SUCCESS;

}

// todo: figure out why the clock doesnt work
void on_tick (int sig)
{
  printf("ASASAS enter function on_tick. running is %d\n",
         running_process_id);
  fflush(stdout);
  if (sig == SIGVTALRM){
    printf("ASASAS enter function on_tick. entered the if\n");
    fflush(stdout);
    threads[running_process_id].quantums++;
    manage_sleepers ();
    schedule (READY);
    total_tick++;
  }
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

  printf("ASASAS enter function set_clock. running is %d.\n",
         running_process_id);
  fflush (stdout);

//  sa.sa_handler = timer_handler;
  sa.sa_handler = &on_tick;
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
  printf("ASASAS enter function look_for_id. running is %d\n",
         running_process_id);
  fflush (stdout);

  for (int i = 1; i < MAX_THREAD_NUM; i++)
  {
    if (threads[i].state == NOTEXISTS)
    {  // sign for empty
      return i;
    }
  }
  fflush (stdout);
  return -1;
}

/**
 * checks if the thread exists. if not - output the relevant output and
 * returns -1. otherwise returns 0.
 * @param tid the id of the thread
 */
int is_exists (int tid)
{
  printf("ASASAS enter function is_exists. running is %d. parameter: %d\n",
         running_process_id, tid);
  if (threads[tid].state == NOTEXISTS)
  {
    printf ("thread library error: thread does`nt exists\n");
    printf ("thread library error: thread does`nt exists\n");
    fflush (stderr);
    return FAIL;
  }
  return SUCCESS;
}

// todo: I have written it late in night, we should check it carefully!
void manage_sleepers ()
{
  printf("ASASAS enter function manage_sleepers. running is %d. sleepers "
         "amount: %d\n",
         running_process_id, get_sleepers_amount());
  fflush(stdout);

  struct node *ptr;
  ptr = head;
  int sleepers_to_delete[MAX_THREAD_NUM];
  int last_delete_index = -1;
  while (ptr != NULL)
  {

    int sleeper_id = ptr->data;
    if (--threads[sleeper_id].remain_sleep_time <= 0)
    {
      if (threads[sleeper_id].state != BLOCKED)
      {
        threads[sleeper_id].state = READY;
        push_to_ready (sleeper_id);
      }
      sleepers_to_delete[++last_delete_index] = sleeper_id;
    }
    ptr = ptr->next;
  }
  while (last_delete_index >= 0)
  {  // delete the awake threads from the list
    delete_fron_sleeping (sleepers_to_delete[last_delete_index--]);
  }
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
// todo: starting the main thread
int uthread_init (int quantum_usecs)
{
  printf("ASASAS enter function uthread_init. running is %d\n",
         running_process_id);
  set_clock (on_tick, quantum_usecs, quantum_usecs);
  // init threads array
  Thread default_thread = {0, NOTEXISTS, NULL, NULL, 1, 0};
  for (int id = 0; id < MAX_THREAD_NUM; id++)
  {
    threads[id] = default_thread;
  }
  threads[0].state = RUN;
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
  printf("ASASAS enter function uthread_spawn. running is %d\n",
         running_process_id);
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
  Thread new_thread = {id, READY, stack, entry_point, 1, 0};
  threads[id] = new_thread;
  setup_thread(id, stack, entry_point);
  current_threads_amount++;
  return push_to_ready (id);
}

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
  printf("ASASAS enter function uthread_terminate. running is %d, parameter "
         "is %d"
         "\n",
         running_process_id, tid);
  if (is_exists (tid) == FAIL)
  {
    return FAIL;
  }

  // delete from ready queue
  if (threads[tid].state == READY)
  {
    remove_from_ready (tid);
  }

  // delete from sleeping list
  if (threads[tid].remain_sleep_time > 0)
  {
    delete_fron_sleeping (tid);
  }

  //delete from threads array
  free (threads[tid].stack);
  threads[tid].state = NOTEXISTS;
  current_threads_amount--;

  if (tid == 0)
  {
    printf("ASASASAS terminating the main thread\n");
    fflush(stdout);
    exit (0);
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
  printf("ASASAS enter function uthread_block. running is %d. parameter is "
         "%d\n",
         running_process_id, tid);
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
  printf("ASASAS enter function uthread_resume. running is %d, parameter is %d"
         "\n",
         running_process_id, tid);
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
  printf("ASASAS enter function uthread_sleep. running is %d, parameter is "
         "%d\n",
         running_process_id, num_quantums);

  if (running_process_id == 0)
  {
    printf ("thread library error: cant put to sleep the main thread\n");
    fflush (stderr);
    return FAIL;
  }
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
  if (threads[running_process_id].state != BLOCKED)
  {
    return schedule (SLEEPING);
  }
  return schedule (BLOCKED);
}

/**
 * @brief Returns the thread ID of the calling thread.
 *
 * @return The ID of the calling thread.
*/
int uthread_get_tid ()
{
  printf("ASASAS enter function uthread_get_tid. running is %d"
         "\n",
         running_process_id);
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
  printf("ASASAS enter function uthread_get_total_quantums. running is %d\n",
         running_process_id);
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
  printf("ASASAS enter function uthread_get_quantums. running is %d\n",
         running_process_id);
  if (is_exists (tid) == FAIL)
  {
    return FAIL;
  }
  return threads[tid].quantums;
}

#endif