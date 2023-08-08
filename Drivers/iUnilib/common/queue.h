//
// Created by dizar on 17.04.2022.
//

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_DATA_SIZE     256

typedef struct
{
    char data[QUEUE_DATA_SIZE];
    size_t length;
} queue_element_t;

typedef struct
{
    queue_element_t *queue;
    size_t size;
    uint32_t in;
    uint32_t out;
} queue_t;

void        queue_add       (queue_t* queue, char *data, size_t length);
char*       queue_get       (queue_t* queue, int *len);
void        queue_flush     (queue_t* queue);
uint32_t    queue_get_qty   (queue_t* queue);


#endif /* _QUEUE_H */
