#include "queue.h"
#include "my_malloc.h"

#include <stdlib.h>

queue_t *allocate_queue(int capacity)
{
    queue_t *q = allocate_memory(sizeof(queue_t));
    q->capacity = capacity;
    q->head = 0;
    q->len = 0;
    q->members = allocate_memory(sizeof(qmem_t) * capacity);

    return q;
}
void reallocate_queue(queue_t *q, int new_capacity)
{
    qmem_t *new_mem = allocate_memory(sizeof(qmem_t) * new_capacity);
    for (int i = 0; i < q->len; ++i)
    {
        new_mem[i] = get_index(q, i);
    }
    q->head = 0;
    deallocate_memory((void **)&q->members);
    q->members = new_mem;
    q->capacity = new_capacity;
}
void free_queue(queue_t *q)
{
    deallocate_memory((void **)&q->members);
    deallocate_memory((void **)&q);
}

void queue_push(queue_t *q, qmem_t *new_mem)
{
    if (q->len >= q->capacity)
    {
        reallocate_queue(q, q->capacity * 2);
    }
    q->members[(q->head + q->len) % q->capacity] = *new_mem;
    q->len++;
}
qmem_t queue_pop(queue_t *q)
{
    if (q->len <= 0)
    {
        qmem_t ret = {.node_index = -1, .prev_feed = -1, .prev_packs = -1};
        return ret;
    }
    qmem_t ret = q->members[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->len--;
    return ret;
}

qmem_t get_index(queue_t *q, int index)
{
    return q->members[(q->head + index) % q->capacity];
}