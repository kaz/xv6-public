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

int sys_getdate(void) {
    struct rtcdate *dp;
    if (argptr(0, (char **)&dp, sizeof(dp)) < 0)
        return -1;
    cmostime(dp);
    return 0;
}

int
a_ge_b(struct rtcdate *a, struct rtcdate *b)
{
  if (a->year   != b->year  ) { return a->year   >= b->year  ; }
  if (a->month  != b->month ) { return a->month  >= b->month ; }
  if (a->day    != b->day   ) { return a->day    >= b->day   ; }
  if (a->hour   != b->hour  ) { return a->hour   >= b->hour  ; }
  if (a->minute != b->minute) { return a->minute >= b->minute; }
                                return a->second >= b->second;
}

int
sys_sleep_until(void)
{
  struct rtcdate now, *until;

  // fetch target date
  if (argptr(0, (char **)&until, sizeof(until)) < 0) {
    return -1;
  }

  // fetch current date
  cmostime(&now);

  // if `until` is before `now`, then return error
  if (a_ge_b(&now, until)) {
    return -1;
  }

  // wait until `now` >= `until`
  acquire(&tickslock);
  while (!a_ge_b(&now, until)) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);

    // renew current date
    cmostime(&now);
  }
  release(&tickslock);

  return 0;
}
