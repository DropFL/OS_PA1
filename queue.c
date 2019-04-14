#include "queue.h"
#include <stdlib.h>

static void __enqueue (ProcQueue* q, Element* e) {
    if (is_empty(q)) {
        q->head = q->tail = e;
        return;
    }

    Element* cur;

    for (cur = q->tail; cur; cur = cur->prev)
        if (!(q->compare) || q->compare(cur->proc, e->proc) <= 0) break;

    if (cur) {
        if (cur != q->tail) {
            cur->next->prev = e;
            e->next = cur->next;
        }
        else q->tail = e;

        cur->next = e;
        e->prev = cur;
    }
    else {
        q->head->prev = e;
        e->next = q->head;
        q->head = e;
    }
}

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
    __enqueue(q, new_elem(p));
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

    switch (p) {
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

int schedule (ProcQueue* q) {
    if (is_empty(q)) return 0;
    return q->scheduler->schedule(peek(q), q->scheduler->arg);
}

void iterate (ProcQueue* q, IterFunc* func, void* arg) {
    if (is_empty(q)) return;

    for (Element *e = q->head, *n = e->next; e; e = n, n = n ? n->next : NULL)
        func(e->proc, arg);
}

void move_head (ProcQueue* from, ProcQueue* to) {
    if (is_empty(from)) return;
    
    Element* target = from->head;

    if (from->head = target->next)
        from->head->prev = NULL;
    else
        from->tail = NULL;
    
    target->next = NULL;
    __enqueue(to, target);
}