#include "scheduler.h"
#include "queue.h"
#include <stdlib.h>

static int round_robin (Process* p, void* arg) {
    int slice = (intptr_t) arg;

    if (CUR_CYCLE(p) < slice)
        slice = CUR_CYCLE(p);

    return (int) slice;
}

static int srtn (Process* p, void* arg) {
    if (!arg) return CUR_CYCLE(p);

    ProcQueue* ready = (ProcQueue *) arg;
    int idx = 0;
    int time = CUR_CYCLE(p);
    
    for (Element* e = ready->head; e && CUR_CYCLE(e->proc) < CUR_CYCLE(p); e = e->next) {
        Process* r = e->proc;
        if (p->queue_idx == r->queue_idx && CUR_CYCLE(p) - CUR_CYCLE(r) > NEXT_CYCLE(r)) {
            time = CUR_CYCLE(r);
            break;
        }
    }

    return time;
}

static int fcfs (Process* p, void* arg) {
    return CUR_CYCLE(p);
}

Scheduler* get_scheduler (Policy p, void* arg) {
    Scheduler* sch = (Scheduler*) malloc(sizeof(Scheduler));

    switch(p) {
        case RR:
            sch->schedule = round_robin;
            sch->arg = arg;
            break;
        case SRTN:
            sch->schedule = srtn;
            sch->arg = arg;
            break;
        case FCFS:
            sch->schedule = fcfs;
            sch->arg = NULL;
            break;
    }

    return sch;
}

Policy get_policy (Scheduler* s) {
    if (s->schedule == round_robin)
        return RR;
    if (s->schedule == srtn)
        return SRTN;
    if (s->schedule == fcfs)
        return FCFS;
}