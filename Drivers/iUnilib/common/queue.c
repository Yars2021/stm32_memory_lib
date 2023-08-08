//
// Created by dizar on 17.04.2022.
//

#define QUEUE_IDX(QUEUE,IN_OUT) (QUEUE->IN_OUT & (QUEUE->size-1))

#include "queue.h"

void queue_add (queue_t* queue, char *data, size_t length)
{
    memcpy(queue->queue[QUEUE_IDX(queue,in)].data, data, length);
    queue->queue[QUEUE_IDX(queue,in)].length = length;
    queue->in++;
}

char* queue_get (queue_t* queue, int *len)
{
    int qty;
    uint32_t idx = QUEUE_IDX(queue, out);
    char *res = 0;

    qty = queue->in - queue->out;
    if (qty)
    {
        res = queue->queue[idx].data;
        *len = queue->queue[idx].length;
        queue->out++;
    }

    return res;
}

uint32_t queue_get_qty (queue_t* queue)
{
    return (queue->in - queue->out);
}

void QUEUE_flush (queue_t* queue)
{
    queue->in = 0;
    queue->out = 0;
}
