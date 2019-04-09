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

    /* `Scheduler` object that defines a strategy to manage `Process`es in this `ProcQueue`. */
    const IProcQueue* const interface;
} ProcQueue;

typedef struct {
    /* Add a `Process` object to the queue. */
    void (*enqueue) (ProcQueue*, Process*);

    /* Remove a `Process` object from the queue and return it. */
    Process* (*dequeue) (ProcQueue*);

    /* Probe if the queue is empty. */
    int (*is_empty) (ProcQueue*);
    
    /* Inherited `IScheduler` interface object. */
    Scheduler* scheduler;
} IProcQueue;