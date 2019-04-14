#include "queue.h"
#include <stdlib.h>

static Element* new_elem (Process* p) {
    Element* elem = (Element*) malloc(sizeof(Element));

    elem->proc = p;
    elem->next = elem->prev = NULL;

    return elem;
}

static Process* del_elem (Element* e) {
    Process* p = e->proc;
    free(e);
    return p;
}

void enqueue (ProcQueue* q, Process* p) {
    Element* elem = new_elem(p);

    if (is_empty(q)) {
        q->head = q->tail = elem;
        return;
    }

    Element* cur;

    for (cur = q->tail; cur; cur = cur->prev)
        if (!(q->compare) || q->compare(cur->proc, p) >= 0) break;

    if (cur) {
        cur->next = elem;
        elem->prev = cur;
        
        if (cur != q->tail) {
            cur->prev = elem;
            elem->next = cur;
        }
        else q->tail = elem;
    }
    else {
        q->head->prev = elem;
        elem->next = q->head;
        q->head = elem;
    }
}

Process* dequeue (ProcQueue* q) {
    Element* e = q->head;

    q->head = e->next;
    if (q->head) q->head->prev = NULL;

    return del_elem(e);
}

int is_empty (ProcQueue* q) {
    return q->head ? 0 : 1;
}

ProcQueue* get_queue (Policy p, void* arg) {
    ProcQueue* q = (ProcQueue*) malloc(sizeof(ProcQueue));

    switch (p)
    {
        case RR:
            q->compare = get_compare(NONE);
            break;
        case SRTN:
            q->compare = get_compare(REMAIN);
            break;
        case FCFS:
            q->compare = get_compare(NONE);
            break;
    }

    q->scheduler = get_scheduler(p, arg);
    q->head = q->tail = NULL;
    return q;
}

Process* peek (ProcQueue* q) {
    return is_empty(q) ? NULL : q->head->proc;
}

void iterate (ProcQueue* q, IterFunc* func, void* arg) {
    if (is_empty(q)) return;

    for (Element* e = q->head; e; e = e->next)
        func(e->proc, arg);
}

int schedule (ProcQueue* q) {
    if (is_empty(q)) return 0;
    return q->scheduler->schedule(peek(q), q->scheduler->arg);
}