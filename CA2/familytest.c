#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid_child1, pid_child2;
  int my_pid = getpid();

  printf(1, "Family Test Program Starting\n");
  printf(1, "Parent PID is: %d\n", my_pid);

  pid_child1 = fork();

  if(pid_child1 == 0){
    sleep(100);
    exit();
  }

  pid_child2 = fork();

  if(pid_child2 == 0){
    sleep(100);
    exit();
  }

  sleep(10);

  printf(1, "\n1. Testing Parent (PID %d)\n", my_pid);
  show_process_family(my_pid);

  printf(1, "\n2. Testing Child 1 (PID %d)\n", pid_child1);
  show_process_family(pid_child1);

  printf(1, "\n3. Testing Child 2 (PID %d)\n", pid_child2);
  show_process_family(pid_child2);

  printf(1, "\n4. Testing Non-existent PID (999)\n");
  int result = show_process_family(999);
  printf(1, "Result for PID 999: %d (Expected: -1)\n", result);

  wait();
  wait();
  
  printf(1, "\nFamily Test Program Finished\n");
  exit();
}