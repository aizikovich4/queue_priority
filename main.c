#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "heap.h"


int main(void)
{
    struct Heap *heap;
    struct Heap_item item;
    heap = heap_create(1000000000);
    heap_insert(heap, 10, "Buy bread");
    heap_insert(heap, 9, "Pay for the Internet");
    heap_insert(heap, 15, "Visit library5");
    heap_insert(heap, 15, "Visit library1");   
    heap_insert(heap, 15, "Visit library3");
    heap_insert(heap, 15, "Visit library4");
    heap_insert(heap, 6, "666");
    heap_insert(heap, 7, "777");
    heap_insert(heap, 9, "999");
    heap_insert(heap, 22, "22222 max");
    heap_insert(heap, 15, "15 last");

    printf("Max element with priority now: %d, %s \n",heap_max(heap).priority, heap_max(heap).value);
    while(heap_size(heap))
    {
        item = heap_removemax(heap);
        printf("Item: %d - %s \n", item.priority, item.value);
    }
    printf("Max element with priority now: %d, %s \n",heap_max(heap).priority, heap_max(heap).value);

    heap_free(heap);



    return 0;
}

