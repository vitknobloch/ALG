#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct
{
    int node_index;
    int prev_feed;
    int prev_packs;
} qmem_t;

typedef struct
{
    int capacity;
    int head;
    int len;
    qmem_t *members;
} queue_t;

queue_t *allocate_queue(int capacity);
void reallocate_queue(queue_t *q, int new_capacity);
void free_queue(queue_t *q);

void queue_push(queue_t *q, qmem_t *new_mem);
qmem_t queue_pop(queue_t *q);

qmem_t get_index(queue_t *q, int index);

#endif