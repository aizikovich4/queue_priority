#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "heap.h"
#define noMUTEX_DEBUG

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


struct Heap *heap_create(int maxsize, void (*callback_events_queue)(int flag))
{
    struct Heap *heap_ptr=NULL;
    if(pthread_mutex_init(&mutex, NULL))
        exit(2);
    heap_ptr = malloc(sizeof(struct Heap));
    if (heap_ptr)
    {
        heap_ptr->maxsize = maxsize;
        heap_ptr->count_items = 0;
        heap_ptr->callback_events_queue = callback_events_queue;
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

static void heap_swap(struct Heap_item *first, struct Heap_item *second)
{
    struct Heap_item tmp = *first;
    *first = *second;
    *second = tmp;
}

struct Heap_item heap_max(const struct Heap *heap)
{
    int res=0;
    struct Heap_item ret_item = {-1,0, NULL};
#ifdef MUTEX_DEBUG_ON
printf("heap_max mutex lock \n ");
#endif
    pthread_mutex_lock(&mutex);
        if (heap->count_items > 0)
        {
            ret_item = heap->items[1];
        }
        else
            printf("Heap is empty\n");

    pthread_mutex_unlock(&mutex);
#ifdef MUTEX_DEBUG_ON
printf("heap_max mutex unlock \n ");
#endif
    return ret_item;

}

void heap_display(struct Heap *heap) {
    int i;
#ifdef MUTEX_DEBUG_ON
printf("heap_display mutex lock \n ");
#endif
heap->callback_events_queue(HIGH_WATER_MARK);

     pthread_mutex_lock(&mutex);
     printf("Queue: ");
        for(i=0; i<=heap->count_items; ++i) {
            printf("%d ", heap->items[i].priority);
        }
        printf("\n");
     pthread_mutex_unlock(&mutex);
#ifdef MUTEX_DEBUG_ON
printf("heap_display mutex unlock \n ");
#endif
}


int heap_insert(struct Heap *heap, int priority, char *value, const unsigned long thread_id)
{
    int i=0;
    struct Heap_item *tmp=NULL;
    #ifdef MUTEX_DEBUG_ON
    printf("heap_insert mutex lock \n ");
    #endif
    pthread_mutex_lock(&mutex);
    if (heap->count_items >= heap->maxsize)
    {
        heap->callback_events_queue(HIGH_WATER_MARK);
        return 1;
    }

    ++heap->count_items;
    heap->items[heap->count_items].priority = priority;
    heap->items[heap->count_items].value = value;
    heap->items[heap->count_items].thread_id = thread_id;

    /* heapify, push to up*/
    for (i = heap->count_items;  i > 1 && heap->items[i].priority > heap->items[i/2].priority;  i = i/2)
    {
        heap_swap(&heap->items[i], &heap->items[i/2]);
    }
    pthread_mutex_unlock(&mutex);
    #ifdef MUTEX_DEBUG_ON
    printf("heap_insert mutex unlock \n ");
    #endif
    return 0;
}

int heap_removemax(struct Heap *heap, struct Heap_item *value)
{
    int largest_index, leftChild;
    int end_heap  ;
    #ifdef MUTEX_DEBUG_ON
        printf("heap_removemax mutex lock \n ");
    #endif
    pthread_mutex_lock(&mutex);

    if(!heap->count_items)
    {
        pthread_mutex_unlock(&mutex);
        #ifdef MUTEX_DEBUG_ON
        printf("heap_removemax mutex unlock \n ");
        #endif
        return 0;
    }
        end_heap = heap->count_items - 1;
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
        *value = heap->items[heap->count_items--];

        if(!heap->count_items)
            heap->callback_events_queue(LOW_WATER_MARK);

    pthread_mutex_unlock(&mutex);
    #ifdef MUTEX_DEBUG_ON
    printf("heap_removemax mutex unlock \n ");
    #endif
    return 1;
}

unsigned int heap_size(struct Heap *heap)
{
    int count=0;
#ifdef MUTEX_DEBUG_ON
    printf("heap_size mutex lock \n ");
#endif
    pthread_mutex_lock(&mutex);
        count = heap->count_items;
    pthread_mutex_unlock(&mutex);
#ifdef MUTEX_DEBUG_ON
    printf("heap_size mutex unlock \n ");
#endif
    return count;
}
