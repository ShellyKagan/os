// main function for testing the uthreads library
#include "uthreads.h"
#include <stdio.h>
#include <unistd.h>
#define QUANTUM_USECS 1

void first_thread(){
  while(1){
    for(int i=0;i<10;i++){
      printf("first %d",i);
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


int main(int argc, char *argv[]){

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

  int tid = uthread_get_tid();
  printf("%d",tid);
  uthread_block(tid);
  sleep(2);
  uthread_resume (tid);
  sleep(2);

  tid = uthread_get_tid();
  printf("%d",tid);
  uthread_sleep(10);
  uthread_block(tid);
  sleep(2);
  uthread_resume (tid);
  sleep(2);

  tid = uthread_get_tid();
  printf("%d",tid);

  uthread_terminate (tid);
  sleep(2);
  //terminates the main thread
  uthread_terminate(0);





}

