#include "process.h"

typedef struct {
    /* Arguments of this `Scheduler`. */
    void *argv;

    /* Function that contains core logic of this `Scheduler`.
       This function executes (i.e. modifies properties of) given `Process`.
       Return value means how much time has passed after scheduling. */
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
Scheduler* get_scheduler (Policy, ...);