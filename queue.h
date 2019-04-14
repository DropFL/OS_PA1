#ifndef Q_GUARD
#define Q_GUARD

#include "process.h"
#include "scheduler.h"

typedef struct _elem {
    /* `Process` object that this `Element` holds */
    Process *proc;
    
    /* Next and previous `Element` object, in double linked list form. */
    struct _elem *next, *prev;
} Element;

typedef struct {
    /* Head and tail `Element` of this `ProcQueue`. */
    Element *head, *tail;

    /* `Process` comparator function that determines prior process */
    ProcCompare* compare;

    /* `Scheduler` object that defines a strategy to manage `Process`es in this `ProcQueue`. */
    Scheduler* scheduler;
} ProcQueue;

typedef void IterFunc (Process*, void* arg);

/* Add a `Process` object to `ProcQueue`. */
void enqueue (ProcQueue*, Process*);

/* Remove a `Process` object from `ProcQueue` and return it. */
Process* dequeue (ProcQueue*);

/* Check if the `ProcQueue` is empty. */
int is_empty (ProcQueue*);

/* Generate a `ProcQueue` object with given `Policy`. */
ProcQueue* get_queue (Policy policy, void* args);

/* Probe the first `Process` object in `ProcQueue`. */
Process* peek (ProcQueue*);

/* Execute the `Scheduler.schedule` function of the `ProcQueue`. */
int schedule (ProcQueue*);

/* Iterate the target `ProcQueue` and execute given function on each `Process` */
void iterate (ProcQueue*, IterFunc*, void* arg);

/* Remove the head of `from` and set it to the head of `to` */
void move_head (ProcQueue* from, ProcQueue* to);

#endif