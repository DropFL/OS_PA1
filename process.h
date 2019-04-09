#include <stdio.h>

typedef struct {
    /* ID of this `Process`. */
    int pid;
    
    /* Index of the queue which this `Process` is (or to be) located. */
    int queue_idx;
    
    /* Arrival time of `Process`. */
    int arrival;
    
    /* No. of cycles. */
    int num_cycles;
    
    /* Index of cycle that this `Process` is passing. */
    int current_cycle;
    
    /* Array of cycles, in `CPU burst - IO burst - CPU burst - ...` order. */
    int* cycles;
} Process;

typedef int ProcCompare (Process*, Process*);

typedef enum {
    /* Set comparison factor as arrival time of a process. */
    ARRIVAL,

    /* Set comparison factor as remaining cycle time of a process. */
    REMAIN
} Factor;

/* Generate a `Process` object from the given file */
Process* make_process (FILE*);

/* Get a process comparator function matches to given factor. */
ProcCompare* get_compare (Factor);

/* Destroy given `Process` object */
void free_process (Process *);