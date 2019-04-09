#include "scheduler.h"
#include "queue.h"

static int round_robin (Process* p, void* arg) {
    int slice = (int) arg;

    if (p->cycles[p->current_cycle] < slice) {
        slice = p->cycles[p->current_cycle];
        p->cycles[p->current_cycle] = 0;
    }
    else p->cycles[p->current_cycle] -= slice;

    return slice;
}

static int srtn (Process* p, void* arg) {
    ProcQueue* ready = (ProcQueue *) arg;

    
}

static int fcfs (Process* p, void* arg) {
    int time = p->cycles[0];

    p->cycles[0] = 0;

    return time;
}