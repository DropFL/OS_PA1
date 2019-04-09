#include "process.h"
#include <stdio.h>
#include <stdlib.h>

static int comp_arr (Process* a, Process* b) {
    return (b->arrival) - (a->arrival);
}

static int comp_rem (Process* a, Process* b) {
    return (b->cycles[b->current_cycle]) - (a->cycles[a->current_cycle]);
}

ProcCompare* get_compare (Factor f) {
    switch (f)
    {
        case ARRIVAL:
            return comp_arr;
        case REMAIN:
            return comp_rem;
        default:
            return NULL;
    }
}

Process* make_process (FILE* f) {
    Process* p = (Process *) malloc(sizeof(Process));

    fscanf(f, "%d %d %d %d", &(p->pid), &(p->queue_idx), &(p->arrival), &(p->num_cycles));
    p->num_cycles = p->num_cycles * 2 - 1;
    p->current_cycle = 0;
    p->cycles = (int *) malloc(sizeof(int) * p->num_cycles);
    
    for (int i = 0; i < p->num_cycles; i ++)
        fscanf(f, "%d", (p->cycles) + i);

    return p;
}

void free_process (Process* p) {
    free(p->cycles);
    free(p);
}