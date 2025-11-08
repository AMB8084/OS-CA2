#include "types.h"
#include "stat.h"
#include "user.h"

void cpu_intensive_task(int pid)
{
    long i;
    for (i = 0; i < 2000000000; i++)
    {
        if (i % 200000000 == 0)
        {
            printf(1, "PID %d: Working...\n", pid);
        }
    }
}

int main(void)
{
    int pid1, pid2;
    int mypid = getpid();

    pid1 = fork();
    if (pid1 == 0)
    {
        int child1_pid = getpid();
        printf(1, "Child 1 (PID %d) created. Will run CPU task.\n", child1_pid);
        cpu_intensive_task(child1_pid);
        printf(1, "* Child 1 (PID %d - LOW PRIORITY) finished. *\n", child1_pid);
        exit();
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        int child2_pid = getpid();
        printf(1, "Child 2 (PID %d) created. Will run CPU task.\n", child2_pid);
        cpu_intensive_task(child2_pid);
        printf(1, "* Child 2 (PID %d - HIGH PRIORITY) finished. *\n", child2_pid);
        exit();
    }
    sleep(200);

    printf(1, "Parent (PID %d): Setting priorities...\n", mypid);
    printf(1, "Parent: Priority for Child 1 (PID %d) set to LOW (2).\n", pid1);
    printf(1, "Parent: Priority for Child 2 (PID %d) set to HIGH (0).\n", pid2);

    set_priority_syscall(pid1, 2);
    set_priority_syscall(pid2, 0);

    wait();
    wait();

    printf(1, "Parent: Both children finished. Test complete.\n");
    exit();
}