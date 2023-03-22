#ifndef _OSM_H
#include "osm.h"
#include <iostream>
#include <cmath>
#include <sys/time.h>

/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time (unsigned int iterations)
{
  struct timeval i_tv;
  struct timeval f_tv;
  unsigned int i = 0;
  iterations = iterations/128;
  gettimeofday (&i_tv, NULL);
  while (i <= iterations)
  {
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
    i += 1;
  }
  gettimeofday (&f_tv, NULL);
  unsigned long long f_ns_time = (
      (unsigned long long) f_tv.tv_sec * pow (10, 9) + (unsigned long
      long) f_tv.tv_usec * pow (10, 3));
  unsigned long long i_ns_time = ((unsigned long long) i_tv.tv_sec * pow
      (10, 9) + (unsigned long
  long) i_tv.tv_usec * pow (10, 3));
  return f_ns_time - i_ns_time;
}

int empty_function(){
  return 0;
}

/* Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time (unsigned int iterations)
{
  struct timeval i_tv;
  struct timeval f_tv;

  unsigned int i = 0;
  unsigned int iterations_div_128 = iterations/128;
  int j = 0;
  gettimeofday (&i_tv, NULL);
  while (i <= iterations_div_128)
  {
    i += 1;
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
    j= empty_function();
  }
  gettimeofday (&f_tv, NULL);
  unsigned long long f_ns_time = (
      (unsigned long long) f_tv.tv_sec * pow (10, 9) + (unsigned long
      long) f_tv.tv_usec * pow (10, 3));
  unsigned long long i_ns_time = ((unsigned long long) i_tv.tv_sec * pow
      (10, 9) + (unsigned long
  long) i_tv.tv_usec * pow (10, 3));
  return f_ns_time - i_ns_time;
}

/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time (unsigned int iterations)
{
  struct timeval i_tv;
  struct timeval f_tv;

  if (iterations == 0){
    return -1;
  }
  unsigned int iterations_div_128 = iterations/128;
  unsigned int i = 0;

  gettimeofday (&i_tv, NULL);
  while (i <= iterations_div_128)
  {
    i += 1;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
    OSM_NULLSYSCALL;
  }
  gettimeofday (&f_tv, NULL);
  unsigned long long f_ns_time = (
      (unsigned long long) f_tv.tv_sec * pow (10, 9) + (unsigned long
      long) f_tv.tv_usec * pow (10, 3));
  unsigned long long i_ns_time = ((unsigned long long) i_tv.tv_sec * pow
      (10, 9) + (unsigned long
  long) i_tv.tv_usec * pow (10, 3));
  return f_ns_time - i_ns_time;
}

int main ()
{
  for(int i=0; i<4;i++){
    std::cout << "operation: " << osm_operation_time (pow (10, 9)) << std::endl;
    std::cout << "empty call: " <<osm_function_time(pow (10, 9)) << std::endl;
    std::cout << "sys call: " <<osm_syscall_time (pow (10, 9)) << std::endl;
  }
  return 0;
}

#endif
