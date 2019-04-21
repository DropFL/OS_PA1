#include <stdlib.h>
#include "MFQ.h"
#include "option.h"

static int last = -1;

static void wait (Process* p, void* arg) {
    void** args = (void**)arg;
    if (p == args[1]) return;
    p->wait += *(int*) args[0];
}

static void elapse (Process* p, void* arg) {
    void** args = (void**)arg;
    int time = *(int*)args[0];
    MFQ* mfq = (MFQ*)args[2];

    if (p == args[1]) return;

    VERBOSE printf("Elapse %d on process %d\n", time, p->pid);

    if (CUR_CYCLE(p) <= time) {
        VERBOSE printf("Process %d's cycle %d has ended.\n", p->pid, p->current_cycle);
        int delta = time - CUR_CYCLE(p);
        CUR_CYCLE(p) = 0;
        p->current_cycle ++;
        if (IS_ACTIVE(p)) {
            VERBOSE printf("Moving process %d from ready queue to queue %d...\n", p->pid, p->queue_idx);
            move_head(mfq->ready_queue, mfq->queues[p->queue_idx]);
            p->wait += delta;
        }
        else if (PROC_END(p)) {
            VERBOSE printf("Process %d just ended.\n", p->pid);
            dequeue(mfq->queues[p->queue_idx]);
        }
        else {
            VERBOSE printf("Moving process %d from queue %d to ready queue...\n", p->pid, p->queue_idx);
            move_head(mfq->queues[p->queue_idx], mfq->ready_queue);
            p->queue_idx = p->queue_idx - (p->queue_idx != 0);
        }
    }
    else {
        CUR_CYCLE(p) -= time;
        VERBOSE printf("Remaining time: %d\n", CUR_CYCLE(p));

        if (IS_ACTIVE(p)) {
            int next_q = p->queue_idx + (p->queue_idx + 1 != mfq->num_queue);
            VERBOSE printf("Moving process %d from queue %d to queue %d...\n", p->pid, p->queue_idx, next_q);
            move_head(mfq->queues[p->queue_idx], mfq->queues[next_q]);
            p->queue_idx = next_q;
        }
    }
}

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
    VERBOSE {
        printf("Setting the policy of queue %d as ", idx);
        switch (p) {
            case RR:
                printf("round robin...\n");
                break;
            case SRTN:
                printf("shortest remaining time next...\n");
                break;
            case FCFS:
                printf("first come first served...\n");
                break;
        }
    }
    mfq->queues[idx] = get_queue(p, arg);
}

int proceed (MFQ* mfq, Process** p, int* t) {
    int i;
    void* args[3] = {t, NULL, mfq};

    if (last < 0) {
        for (i = 0; i < mfq->num_queue; i ++)
            if (!is_empty(mfq->queues[i])) break;
    }
    else {
        VERBOSE printf("Queue %d was saved for scheduling.\n", last);
        i = last;
    }

    last = -1;
    
    if (i == mfq->num_queue) {
        VERBOSE printf("There are no queued processes.\n");
        if (is_empty(mfq->ready_queue)) {
            VERBOSE printf("Every process is ended.\n");
            return 0;
        }
        
        *p = NULL;
        *t = CUR_CYCLE(peek(mfq->ready_queue));
    }
    else {
        VERBOSE printf("Scheduling %d queue...\n", i);
        *p = peek(mfq->queues[i]);
        *t = schedule(mfq->queues[i]);
        VERBOSE printf("Process %d is scheduled for %d.\n", (*p)->pid, *t);
        if (CUR_CYCLE(*p) > *t && get_policy(mfq->queues[(*p)->queue_idx]->scheduler) == SRTN) {
            last = (*p)->queue_idx;
            VERBOSE printf("Process %d has preempted, saving queue %d for next scheduling...\n", (*p)->pid, last);
        }
        elapse(*p, args);
        args[1] = *p;
    }
    VERBOSE printf("Waiting for %d on every active queue...\n", *t);
    for (int i = 0; i < mfq->num_queue; i ++)
        iterate(mfq->queues[i], wait, args);

    VERBOSE printf("Elapsing %d on ready queue...\n", *t);
    iterate(mfq->ready_queue, elapse, args);

    VERBOSE("proceed ended.\n");

    return 1;
}