#pragma once

struct Heap_item {
    int priority;               // priority of elemen
    char *value;                // data
};

struct Heap {
    int maxsize;                 // max heap size
    int count_items;             // count of elements ih heap
    struct Heap_item *items;     //heap elements, we will stories them from first position, not from zero position
};

struct Heap *heap_create(int maxsize);
void        heap_free(struct Heap *h);
void        heap_swap(struct Heap_item *first, struct Heap_item *second);
struct Heap_item heap_max(const struct Heap *heap);
void        heap_display(struct Heap *heap);
int         heap_insert(struct Heap *heap, int priority, char *value);
struct Heap_item heap_removemax(struct Heap *heap);
unsigned int heap_size(struct Heap *heap);
