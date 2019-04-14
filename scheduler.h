#ifndef SCH_GUARD
#define SCH_GUARD

#include "process.h"

typedef struct {
    /* Arguments of this `Scheduler`. */
    void *arg;

    /* Function that contains core logic of this `Scheduler`.
       This function calculates the scheduled time of given `Process`.
       However, it does not modify any properties of any object. */
    int (*schedule) (Process*, void*);
} Scheduler;

typedef enum {
    /* `RR` is an abbreviation of **R**ound **R**obin.
       This policy needs `time_slice` parameter to use. */
    RR,

    /* `SRTN` is an abbreviation of **S**hortest **R**emaining **T**ime **N**ext.
       This policy needs `ready_queue` parameter since it is preemptive. */
    SRTN,
    
    /* `FCFS` is an abbreviation of **F**irst **C**ome **F**irst **S**erved.
       This policy does not require any parameter. */
    FCFS
} Policy;

/* Get a process comparator function matches to given factor. */
Scheduler* get_scheduler (Policy, void*);

#endif