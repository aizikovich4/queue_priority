#pragma once

#define LOW_WATER_MARK  0
#define HIGH_WATER_MARK 1

struct Heap_item {
    int priority;               // priority of elemen
    pthread_t thread_id;
    char *value;                // data
};

struct Heap {
    int maxsize;                 // max heap size
    int count_items;             // count of elements ih heap
    struct Heap_item *items;     //heap elements, we will stories them from first position, not from zero position
    void (*callback_events_queue)(int); //callback for calling code
};

struct Heap     *heap_create(int maxsize, void (*callback_events_queue)(int));
void            heap_free(struct Heap *h);
static void     heap_swap(struct Heap_item *first, struct Heap_item *second);
struct Heap_item heap_max(const struct Heap *heap);
void            heap_display(struct Heap *heap);
int             heap_insert(struct Heap *heap, int priority, char *value, const unsigned long thread_id);
int             heap_removemax(struct Heap *heap, struct Heap_item *value);
unsigned int    heap_size(struct Heap *heap);
