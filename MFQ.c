#include <stdlib.h>
#include "MFQ.h"

void enqueue (ProcQueue *q, Process *proc) {
    Element *elem = (Element *) malloc(sizeof(Element));
    elem->proc = proc;
    elem->next = elem->prev = NULL;
    
    if (q->head) {
        q->tail->next = elem;
        elem->prev = q->tail;
        q->tail = elem;
    }
    else q->head = q->tail = elem;
}

Process *dequeue (ProcQueue *q) {
    Element *elem = q->head;
    Process *proc = elem->proc;

    q->head = elem->next;

    free(elem);

    return proc;
}