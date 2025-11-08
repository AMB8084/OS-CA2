#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_grep_syscall(void)
{
    char *keyword;
    char *filename;
    char *user_buffer;
    int buffer_size;

    char *line_buf;
    struct inode *ip;
    int offset = 0;
    int line_pos = 0;
    int found = 0;
    int n;
    char c;

    if (argstr(0, &keyword) < 0 ||
        argstr(1, &filename) < 0 ||
        argint(3, &buffer_size) < 0 ||
        argptr(2, &user_buffer, buffer_size) < 0)
    {
        return -1;
    }

    if ((line_buf = kalloc()) == 0)
    {
        return -1;
    }

    begin_op();

    if ((ip = namei(filename)) == 0)
    {
        end_op();
        kfree(line_buf);
        return -1;
    }
    ilock(ip);

    while ((n = readi(ip, &c, offset, 1)) == 1)
    {
        offset++;

        if (c == '\n')
        {
            line_buf[line_pos] = '\0';
            if (k_strstr(line_buf, keyword))
            {
                found = 1;
                break;
            }
            line_pos = 0;
        }
        else
        {
            if (line_pos < PGSIZE - 1)
            {
                line_buf[line_pos++] = c;
            }
        }
    }

    if (!found && line_pos > 0)
    {
        line_buf[line_pos] = '\0';
        if (k_strstr(line_buf, keyword))
        {
            found = 1;
        }
    }

    iunlockput(ip);
    end_op();

    if (found)
    {
        int len_to_copy = line_pos;
        if (len_to_copy >= buffer_size)
        {
            len_to_copy = buffer_size - 1;
        }

        if (copyout(myproc()->pgdir, user_buffer, line_buf, len_to_copy) < 0)
        {
            kfree(line_buf);
            return -1;
        }
        
        char null_term = '\0';
        if (copyout(myproc()->pgdir, user_buffer + len_to_copy, &null_term, 1) < 0)
        {
            kfree(line_buf);
            return -1;
        }

        kfree(line_buf);
        return len_to_copy;
    }

    kfree(line_buf);
    return -1;
}

int sys_set_priority_syscall(void)
{
    int pid;
    int priority;

    if (argint(0, &pid) < 0)
        return -1;
    if (argint(1, &priority) < 0)
        return -1;

    return setpriority(pid, priority);
}







int
sys_arithmeticsyscall(void)
{
  struct proc *curproc = myproc();
  int a, b, result;
  a = curproc->tf->ebx;
  b = curproc->tf->ecx;
  result = (a + b) * (a - b);
  cprintf("Calc: (%d+%d)*(%d-%d)=%d\n", a, b, a, b, result);
  return result;
}



int
sys_show_process_family(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;

  return dump_process_family(pid);
}