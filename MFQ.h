#include "queue.h"

typedef struct {
    /* No. of `ProcQueue`s. */
    int num_queue;

    /* Array of `ProcQueue`s. */
    ProcQueue **queues;

    /*  */
} MFQ;