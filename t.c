#define _POSIX_SOURCE

#define _GNU_SOURCE
#include <signal.h>

#include <sys/wait.h> 
#include<unistd.h>  
#include<stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <errno.h>
#define gettid() syscall(__NR_gettid)

#define SCHED_DEADLINE	6
#define SCHED_NORMAL 0

/* XXX use the proper syscall numbers */
#ifdef __x86_64__
#define __NR_sched_setattr		314
#define __NR_sched_getattr		315
#endif

#ifdef __i386__
#define __NR_sched_setattr		351
#define __NR_sched_getattr		352
#endif

#ifdef __arm__
#define __NR_sched_setattr		380
#define __NR_sched_getattr		381
#endif


struct sched_attr {
  __u32 size;
  
  __u32 sched_policy;
  __u64 sched_flags;
  
  /* SCHED_NORMAL, SCHED_BATCH */
  __s32 sched_nice;
  
  /* SCHED_FIFO, SCHED_RR */
  __u32 sched_priority;
  
  /* SCHED_DEADLINE (nsec) */
  __u64 sched_runtime;
  __u64 sched_deadline;
  __u64 sched_period;
};

int sched_setattr(pid_t pid,
		  const struct sched_attr *attr,
		  unsigned int flags)
{
  return syscall(__NR_sched_setattr, pid, attr, flags);
}

int sched_getattr(pid_t pid,
		  struct sched_attr *attr,
		  unsigned int size,
		  unsigned int flags)
{
  return syscall(__NR_sched_getattr, pid, attr, size, flags);
}  


int main(int argc, char *argv[])
{
  int i;
  
  pid_t pid;
  
  
  printf("printed from parent [%d]\n", getpid());
  pid = fork();
  
  if (pid < 0)
  {
    perror("fork");
    exit(1);
  }
  
  if (pid == 0) //child
  {
    printf("printed from child [%d]\n", getpid());
    pid_t my_id;
    
    int status;
    
    
    struct sched_attr attr;
    int x = 0,result = 0;
    
    //unsigned int flags = 0;
    
    my_id = getpid();
#if 0    
    attr.size = sizeof(attr);
    attr.sched_flags = 0;
    attr.sched_nice = 0;
    attr.sched_priority = 0;
    
    
    
    attr.sched_runtime = 30 * 1000 * 1000;
    attr.sched_period = attr.sched_deadline = 100 * 1000 * 1000;//changing to edf
    printf("changing to EDF \n") ;
    attr.sched_policy = SCHED_DEADLINE;
    result = sched_setattr(my_id, &attr, flags);
    
    
    if (result < 0) {
      
      perror("sched_setattr");
      exit(-1);
  }
  
  printf("\n1-- Syscall returned%d\n", result);
#endif
  
  cpu_set_t  mask;
  CPU_ZERO(&mask);

  /* set 0th core: the numbering is starts from 0 to 3 for a four core machine */
  //CPU_SET(0, &mask);
  
  /* set 1st core: the numbering is starts from 0 to 3 for a four core machine */
  CPU_SET(1, &mask);
  
  
  printf("%d -- set affinity = %d\n",__LINE__, mask);	
  result = sched_setaffinity(my_id, sizeof(mask), &mask);
  printf("%d -- ret = %d\n",__LINE__, result);	 
  
  int counter=0;
  printf("entering loop \n");
  while(1) {
    if (counter > 100000) {
      printf("reached 10000 \n");
      counter = 0; /* reset */
      sleep(1); /* sleep for 1 sec */
    }
    counter++;
  }   
  
  
  }    
  
  
  
}
