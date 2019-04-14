#ifndef MFQ_GUARD
#define MFQ_GUARD

#include "queue.h"

typedef struct _mfq {
    /* No. of `ProcQueue`s. */
    int num_queue;

    /* Array of `ProcQueue`s. */
    ProcQueue** queues;

    /* Queue of non-CPU-bursting `Process`es. */
    ProcQueue* ready_queue;
} MFQ;

/* Initializes given `MFQ` with given no. of queues. */
MFQ* get_mfq (int queues);

/* Initializes selected queue with given arguments. */
void set_queue (MFQ*, int, Policy, void*);

/* Schedule an `MFQ` and save info of the operation.
   Return value is true if some task is done. */
int proceed (MFQ* mfq, Process** p, int* t);

#endif