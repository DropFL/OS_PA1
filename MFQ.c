#include <stdlib.h>
#include <stdarg.h>
#include "MFQ.h"

MFQ* get_mfq (int n) {
    MFQ* mfq = (MFQ*) malloc(sizeof(MFQ));

    mfq->num_queue = n;
    mfq->queues = (ProcQueue**) malloc(sizeof(ProcQueue*) * n);

    for (int i = 0; i < n; i ++)
        mfq->queues[i] = NULL;
    
    mfq->ready_queue = get_queue(SRTN, NULL);

    return mfq;
}

void set_queue (MFQ* mfq, int idx, Policy p, void* arg) {
    if (idx >= mfq->num_queue) return;

    mfq->queues[idx] = get_queue(p, arg);
}

int proceed (MFQ* mfq, Process** p, int* t) {
    int i;

    for (i = 0; i < mfq->num_queue; i ++)
        if (!is_empty(mfq->queues[i])) break;
    
    if (i == mfq->num_queue) {
        if (is_empty(mfq->ready_queue))
            return 0;
        
        *p = NULL;
        Process* nxt = peek(mfq->ready_queue);
        *t = CUR_CYCLE(nxt);
    }
    else {
        *p = peek(mfq->queues[i]);
        CUR_CYCLE(*p) -= (*t = schedule(mfq->queues[i]));

        int next_idx;
        ProcQueue* next_q;

        if (CUR_CYCLE(*p) <= 0) {
            next_idx = (*p)->queue_idx - ((*p)->queue_idx != 0);
            next_q = mfq->ready_queue;
            (*p)->current_cycle ++;
        }
        else {
            next_idx = (*p)->queue_idx + ((*p)->queue_idx < mfq->num_queue - 1);
            next_q = mfq->queues[next_idx];
        }

        (*p)->queue_idx = next_idx;
        dequeue(mfq->queues[i]);

        if (!PROC_END(*p))
            enqueue(next_q, *p);
    }

    // TODO

    return 1;
}