#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

struct Heap *heap_create(int maxsize)
{
    struct Heap *heap_ptr=NULL;
    heap_ptr = malloc(sizeof(struct Heap));
    if (heap_ptr)
    {
        heap_ptr->maxsize = maxsize;
        heap_ptr->count_items = 0;
        heap_ptr->items = malloc(sizeof(struct Heap_item) * maxsize );
        if (!heap_ptr->items)
        {
            exit(1);
        }
       // memset(heap_ptr->items, -1, sizeof(struct Heap_item) * maxsize);
        return heap_ptr;
    }
    else
        exit(1);

}

void heap_free(struct Heap *h)
{
    free(h->items);
    free(h);
}

void heap_swap(struct Heap_item *first, struct Heap_item *second)
{
    struct Heap_item tmp = *first;
    *first = *second;
    *second = tmp;
}

struct Heap_item heap_max(const struct Heap *heap)
{
    struct Heap_item erritem = {-1, NULL};
    if (heap->count_items > 0)
        return heap->items[1];
    else
    {
        printf("Heap is empty\n");
        return erritem;
    }
}

void heap_display(struct Heap *heap) {
    int i;
    for(i=0; i<=heap->count_items; ++i) {
        printf("%d ", heap->items[i].priority);
    }
    printf("\n");
}


int heap_insert(struct Heap *heap, int priority, char *value)
{
    int i=0;
    struct Heap_item *tmp=NULL;
    if (heap->count_items >= heap->maxsize)
    {
        // try to get new memory for our heap
        tmp = realloc(heap->items, sizeof(int) * heap->count_items);
        if(!tmp)
            return -1;
        //all good, continue to add item
        heap->items = tmp;  //Our heap have a new place in memory!
        ++heap->maxsize;
    }

    ++heap->count_items;
    heap->items[heap->count_items].priority = priority;
    heap->items[heap->count_items].value = value;

    /* heapify, push to up*/
    for (i = heap->count_items;  i > 1 && heap->items[i].priority > heap->items[i/2].priority;  i = i/2)
    {
        //printf("before:");
       // heap_display(heap);
        heap_swap(&heap->items[i], &heap->items[i/2]);
        //printf("after:");
        //heap_display(heap);
    }
    return 0;
}

struct Heap_item heap_removemax(struct Heap *heap)
{
    int largest_index, leftChild;
    int end_heap = heap->count_items - 1;
    heap_swap(&heap->items[1], &heap->items[heap->count_items]);  //swap with head with last
    for (largest_index = 1; 2*largest_index <= end_heap;  largest_index = leftChild)  //try to rebuild heap
    {
        leftChild = 2 * largest_index;                       //index for point out to leaves
        if (leftChild < end_heap && heap->items[leftChild].priority <  heap->items[leftChild + 1].priority)
        {
            leftChild++;
        }
        if (heap->items[largest_index].priority >= heap->items[leftChild].priority)
        {
            break;
        }
        heap_swap(&heap->items[largest_index], &heap->items[leftChild]);
    }
    return heap->items[heap->count_items--];
}

unsigned int heap_size(struct Heap *heap)
{
    return heap->count_items;
}
