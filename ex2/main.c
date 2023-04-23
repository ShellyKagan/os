// main function for testing the uthreads library
#include "uthreads.h"
#include <stdio.h>
#include <unistd.h>

#define QUANTUM_USECS 1

void first_thread(){
  while(1){
    for(int i=0;i<10;i++){
      printf("first %d",i);
      fflush(stdout);
    }
  }
}

void second_thread(){
  while(1){
    for(int i=10;i<20;i++){
      printf("second %d", i);
    }
  }
}

void third_thread(){
  while(1){
    for(int i=20;i<30;i++){
      printf("third %d",i);
    }
  }
}


int main(){

  display_status();

  uthread_init(QUANTUM_USECS);
  // test adding & delete threads
  thread_entry_point first_entry_point = &first_thread;
  uthread_spawn(first_entry_point);
  sleep(1);
  thread_entry_point second_entry_point = &second_thread;
  uthread_spawn(second_entry_point);
  sleep(1);
  thread_entry_point third_entry_point = &third_thread;
  uthread_spawn(third_entry_point);
  sleep(1);

  display_status();

  int tid = uthread_get_tid();
  printf("blocking thread with id: %d", tid);
  fflush(stdout);
  uthread_block(1);
  sleep(2);
  uthread_resume(tid);
  sleep(2);

  display_status();

  tid = uthread_get_tid();
  printf("%d",tid);
  fflush(stdout);

  uthread_sleep(10);
  tid = uthread_get_tid();
  printf("blocking thread with id: %d", tid);
  fflush(stdout);
  uthread_block(tid);
  sleep(2);
  uthread_resume (tid);
  sleep(2);

  display_status();
  sleep(8);
  display_status();

  tid = uthread_get_tid();
  printf("%d",tid);
  fflush(stdout);

  //terminates the main thread
  uthread_terminate(0);

  display_status();






}

