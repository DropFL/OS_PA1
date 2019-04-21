#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "option.h"

static int comp_rem (Process* a, Process* b) {
    return CUR_CYCLE(a) - CUR_CYCLE(b);
}

static int comp_non (Process* a, Process* b) {
    return 0;
}

ProcCompare* get_compare (Factor f) {
    switch (f)
    {
        case REMAIN:
            return comp_rem;
        default:
            return comp_non;
    }
}

Process* read_process (FILE* f) {
    Process* p = (Process *) malloc(sizeof(Process));

    if (fscanf(f, "%d %d %d %d", &(p->pid), &(p->queue_idx), &(p->arrival), &(p->num_cycles)) != 4) {
        fprintf(stderr, "read_process error: failed to read a Process info from the file.\n");
        free(p);
        return NULL;
    }
    p->num_cycles = p->num_cycles * 2;
    p->current_cycle = p->arrival == 0;
    p->term = p->wait = 0;
    p->cycles = (int *) malloc(sizeof(int) * p->num_cycles);
    p->cycles[0] = p->arrival;
    
    for (int i = 1; i < p->num_cycles; i ++)
        if (fscanf(f, "%d", (p->cycles) + i) != 1) {
            fprintf(stderr, "read_process error: failed to read the cycle %d from the file.\n", i);
            free(p);
            return NULL;
        }
    
    VERBOSE {
        printf("A process is generated succesfully.\n", p->pid);
        print_process(p);        
    }

    return p;
}

void free_process (Process* p) {
    free(p->cycles);
    free(p);
}

void print_process (Process *p) {
    printf("Process %d info:\n", p->pid);
    printf("\tQueue index: %d\n", p->queue_idx);
    printf("\t# of cycles: %d\n", p->num_cycles);
    printf("\tCurrent state: %s\n", (p->current_cycle == 0) ? "not arrived yet" : IS_ACTIVE(p) ? "CPU burst" : "I/O burst");
    printf("\tCurrent cycle index: %d\n", p->current_cycle);
    printf("\tTime to complete current cycle: %d\n", CUR_CYCLE(p));
}