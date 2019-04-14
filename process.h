#ifndef PROC_GUARD
#define PROC_GUARD

#include <stdio.h>

#define CUR_CYCLE(p) (p)->cycles[(p)->current_cycle]
#define NEXT_CYCLE(p) (p)->cycles[(p)->current_cycle + 1]
#define PROC_END(p) ((p)->current_cycle >= (p)->num_cycles)

typedef struct {
    /* ID of this `Process`. */
    int pid;
    
    /* Index of the queue which this `Process` is (or to be) located. */
    int queue_idx;
    
    /* No. of cycles. */
    int num_cycles;
    
    /* Index of cycle that this `Process` is passing. */
    int current_cycle;
    
    /* Array of cycles, in `Arriaval - CPU burst - IO burst - CPU burst - ...` order. */
    int* cycles;
} Process;

typedef int ProcCompare (Process* a, Process* b);

typedef enum {
    /* Does not compare process at all. */
    NONE,

    /* Set comparison factor as remaining cycle time of a process. */
    REMAIN
} Factor;

/* Generate a `Process` object from the given file. */
Process* read_process (FILE*);

/* Get a process comparator function matches to given factor. */
ProcCompare* get_compare (Factor);

/* Destroy given `Process` object. */
void free_process (Process*);

/* Print info about given `Process` object in pretty way. */
void print_process (Process*);

#endif