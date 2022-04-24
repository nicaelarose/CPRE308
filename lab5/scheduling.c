/*******************************************************************************
 * *
 * * CprE 308 Scheduling Lab
 * *
 * * scheduling.c
 * * last updated 2020-03-01 - mlw
 * *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROCESSES 20
#define NUM_SCHEDULERS 4

#define SCHEDULER_NAME_LEN 30

/* Defines a simulated process */
typedef struct process {
  int arrivaltime;    /* Time process arrives and wishes to start */
  int starttime;      /* Time process actually starts */
  int runtime;        /* Time process requires to complete job */
  int remainingtime;  /* Runtime remaining before process completes */
  int endtime;        /* Time process finishes */
  
  int priority;       /* Priority of the process */

  int running;        /* Convenience - indicates if the process is currently running */
  int finished;       /* Convenience - indicates if the process has finished */
} process;

/* Defines a simulated scheduler */
typedef struct scheduler
{
  int (* func) (process [], int); /* Function to run for the scheduler */
  char name[SCHEDULER_NAME_LEN];  /* Name of the scheduler, for pretty printing */
} scheduler;

/* Prototypes of scheduler functions */
int first_come_first_served(process proc[], int t);
int shortest_remaining_time(process proc[], int t);
int round_robin(process proc[], int t);
int round_robin_priority(process proc[], int t);

/* Main, contains most of the simulation code */
int main() {
  int i,j;
  process proc[NUM_PROCESSES],        /* List of processes */
            proc_copy[NUM_PROCESSES]; /* Backup copy of processes */
            
  /* Initialize the schedulers */
  scheduler schedulers[NUM_SCHEDULERS];
  schedulers[0].func = first_come_first_served;
  strncpy(schedulers[0].name, "First come first served", SCHEDULER_NAME_LEN);
  schedulers[1].func = shortest_remaining_time;
  strncpy(schedulers[1].name, "Shortest remaining time", SCHEDULER_NAME_LEN);
  schedulers[2].func = round_robin;
  strncpy(schedulers[2].name, "Round robin", SCHEDULER_NAME_LEN);
  schedulers[3].func = round_robin_priority;
  strncpy(schedulers[3].name, "Round robin with priority", SCHEDULER_NAME_LEN);

  /* Seed random number generator */
  //srand(time(0));    /* Use this seed to test different inputs */
  srand(0xC0FFEE);     /* Use this seed to test fixed input */

  /* Initialize process structures */
  for(i=0; i<NUM_PROCESSES; i++)
  {
    proc[i].arrivaltime = rand() % 100;
    proc[i].runtime = (rand() % 30) + 10;
    proc[i].remainingtime = proc[i].runtime;
    proc[i].priority = rand() % 3;
	
    proc[i].starttime = -1;
    proc[i].endtime = -1;
    proc[i].running = 0;
    
    proc[i].finished = 0;
  }

  /* Print process values */
  printf("Process\tarrival\truntime\tpriority\n");
  for(i = 0; i < NUM_PROCESSES; i++) {
    printf("%d\t%d\t%d\t%d\n", i, proc[i].arrivaltime, proc[i].runtime,
           proc[i].priority);
  }
  
  /* Run simulation for each scheduler */
  for(i = 0; i < NUM_SCHEDULERS; i++) {
    int num_finished = 0;
    int current_time = 0;
    int prev_pid = -1;
    int total_turnaround_time = 0;
    printf("\n\n--- %s\n", schedulers[i].name);
    
    /* Use a copy of the processes array */
    memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(process));
    
    /* Run the simulation until all processes finish */
    while(num_finished < NUM_PROCESSES) {
      process * p;
      
      /* Call the scheduler */
      int next_pid = schedulers[i].func(proc_copy, current_time);
      
      /* If there's no process to run, just move time forward */
      if(next_pid < 0) {
        current_time += 1;
        continue;
      }
      
      /* Convenience - use p to reference the proc */
      p = &proc_copy[next_pid];
      
      /* If the process just started, print a message */
      if(p->starttime == -1) {
        printf("%03d: Process %d started\n", current_time, next_pid);
        p->starttime = current_time;
      }
      
      /* Update which process is running */
      if(prev_pid >= 0) {
        proc_copy[prev_pid].running = 0;
      }
      prev_pid = next_pid;
      p->running = 1;
      
      /* Move time forward */
      current_time += 1;
      
      /* Update remaining time of the process that just ran */
      p->remainingtime -= 1;
      
      /* If the process already finished, it shouldn't have been chosen */
      if(p->remainingtime < 0) {
        printf("Error: tried to run finished process %d\n", next_pid);
        continue;
      /* If the process just finished, print a message and do bookkeeping */
      } else if(p->remainingtime == 0) {
        printf("%03d: Process %d finished\n", current_time, next_pid);
        p->endtime = current_time;
        p->finished = 1;
        num_finished += 1;
        /* Keep a running total of turnaround time */
        total_turnaround_time += p->endtime - p->arrivaltime;
      }
    }
    
    /* Print average turnaround time */
    printf("Average turnaround time: %.01lf seconds\n", (double)total_turnaround_time / NUM_PROCESSES);
  }

  return 0;
}

/* ----------------------Schedulers------------------------------*/
/* The scheduler being simulated is called each timestep.
   The scheduler function should return the index of the process 
   from proc[] that should run next. Parameter t is the current time 
   of the simulation. Note that proc[] contains all processes, 
   including those that have not yet arrived and those that are finished.
   You are responsible for ensuring that you schedule a "ready"
   process. If no processes are ready, return -1.
*/
   
int first_come_first_served(process proc[], int t)
{
  /* TODO: Implement scheduling algorithm here */
  int i = 0;
  int first = -1;
  int arrival = 0;

  while(i < NUM_PROCESSES && first == -1)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0)
    {
      first = i;
      arrival = proc[i].arrivaltime;
    }
    i++;
  }
  if(first == -1)
    return first;
  for(i = first; i < NUM_PROCESSES; i++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && proc[i].arrivaltime < arrival)
    {
      arrival = proc[i].arrivaltime;
      first = i;
    }
  }
  return first; 
}

int shortest_remaining_time(process proc[], int t)
{
  /* TODO: Implement scheduling algorithm here */
  int i = 0;
  int first = -1;
  int arrival;

  while(i < NUM_PROCESSES && first == -1)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0)
    {
      first = i;
      arrival = proc[i].remainingtime;
    }
    i++;
  }
  if(first == -1)
    return first;
  for(i = first; i < NUM_PROCESSES; i++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && proc[i].remainingtime < arrival)
    {
      arrival = proc[i].remainingtime;
      first = i;
    }
  }
  return first; 
}
int lastPID = -1;

int round_robin(process proc[], int t)
{
  /* TODO: Implement scheduling algorithm here */
  //HINT: consider using a static variable to keep track of the previously scheduled process

  int i = 0;
  int first = -1;
  while(i < NUM_PROCESSES && first == -1 && lastPID == -1)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0)
    {
      first = i;
      lastPID = i;
      return i;
    }
    i++;
  }
  if(i == -1)
    return first;
  for(i = lastPID; i < NUM_PROCESSES; i++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && lastPID != i)
    {
      lastPID = i;
      first = i;
      return first;
    }
  }
  for(i = 0; i < lastPID; i ++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && lastPID != i)
    {
      lastPID = i;
      first = i;
      return first;
    }
  }
  return lastPID;
}
int last = -1;
	

int round_robin_priority(process proc[], int t)
{
  /* TODO: Implement scheduling algorithm here */
  int i = 0;
  int level0 = -1;
  int level1 = -1;
  int level2 = -1;


  while(i < NUM_PROCESSES && level0 == -1 && last == -1)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && proc[i].priority == 0)
    {
      level0 = i;
    }
    i++;
  }
  while(i < NUM_PROCESSES && level1 == -1 && last == -1)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && proc[i].priority == 1)
    {
      level1 = i;
    }
    i++;
  }
  while(i < NUM_PROCESSES && level2 == -1 && last == -1)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && proc[i].priority == 2)
    {
      level2 = i;
    }
    i++;
  }
  if(!level0 && !level1 && !level2)
    return -1;
  if(!last)
  {
    if(level2 > -1)
    {
      last = level2;
      return last;
    }
    if(level1 > -1)
    {
      last = level1;
      return last;
    }
    if(level0 > -1)
    {
      last = level0;
      return last;
    }
  }
  for(i = last; i < NUM_PROCESSES; i++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && last != i && proc[i].priority == 2)
    {
      last = i;
      return last;
    }
  }
  for(i = 0; i < last; i ++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && last != i && proc[i].priority == 2)
    {
       last = i;
      return last;
    }
  }
  if(proc[last].priority == 2 && proc[last].remainingtime > 0)
    return last;


  for(i = last; i < NUM_PROCESSES; i++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && last != i && proc[i].priority == 1)
    {
       last = i;
       return last;
    }
  }
  for(i = 0; i < last; i ++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && last != i && proc[i].priority == 1)
    {
       last = i;
      return last;
    }
  }
  if(proc[last].priority == 1 && proc[last].remainingtime > 0)
    return last;
  

  for(i = last; i < NUM_PROCESSES; i++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && last != i && proc[i].priority == 0)
    {
       last = i;
      return last;
    }
  }
  for(i = 0; i < last; i ++)
  {
    if(proc[i].arrivaltime <= t && proc[i].remainingtime > 0 && last != i && proc[i].priority == 0)
    {
       last = i;
      return last;
    }
  }
  if(proc[last].priority == 0 && proc[last].remainingtime > 0)
    return last;
  return last;
}
