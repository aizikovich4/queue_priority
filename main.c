#include <stdio.h>
#include <stdlib.h>

struct Heap_item {
    int priority;               /* priority of elemen */
    char *value;                /* data*/
};

struct Heap {
    int maxsize;         /* max heap size */
    int count_items;             /* count of elements ih heap */
    struct Heap_item *items;
};

struct Heap *heap_create(int maxsize)
{
    struct Heap *heap_ptr;
    heap_ptr = malloc(sizeof(struct Heap));
    if (heap_ptr != NULL)
    {
        heap_ptr->maxsize = maxsize;
        heap_ptr->count_items = 0;
        heap_ptr->items = malloc(sizeof(struct Heap_item) * (maxsize + 1));

        if (heap_ptr->items == NULL)
        {
            exit(3);
        }
    }
    return heap_ptr;
}

void heap_free(struct Heap *h)
{
    free(h->items);
    free(h);
}

void heap_swap(struct Heap_item *a, struct Heap_item *b)
{
    struct Heap_item temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

struct Heap_item heap_max(struct Heap *h)
{
    struct Heap_item erritem = {-1, NULL};
    if (h->count_items > 0)
    {
        return h->items[1];
    }
    else
    {
        fprintf(stderr,
        "heap: Heap is empty.\n");
        return erritem;
    }
}

int heap_insert(struct Heap *heap, int priority, char *value)
{
    int i;
    if (heap->count_items >= heap->maxsize)
    {
        fprintf(stderr,
        "heap: Heap overflow.\n");
        return -1;
    }
    heap->count_items++;
    heap->items[heap->count_items].priority = priority;
    heap->items[heap->count_items].value = value;
    /* Продвигаем элемент вверх */
    for (i = heap->count_items;    i > 1 && heap->items[i].priority > heap->items[i / 2].priority;  i = i / 2)
    {
        heap_swap(&heap->items[i], &heap->items[i / 2]);
    }
    return 0;
}

struct Heap_item heap_removemax(struct Heap *heap)
{
    int k, n, j;
    heap_swap(&heap->items[1],
    &heap->items[heap->count_items]);
    for (k = 1, n = heap->count_items - 1; 2 * k <= n;  k = j)
    {
        j = 2 * k;
        if (j < n && heap->items[j].priority <  heap->items[j + 1].priority)
        {
            j++;
        }
        if (heap->items[k].priority >= heap->items[j].priority)
        {
            break;
        }
        heap_swap(&heap->items[k], &heap->items[j]);
    }
    return heap->items[heap->count_items--];
}



int main(void)
{
    struct Heap *heap;
    struct Heap_item item;
    heap = heap_create(20);
    heap_insert(heap, 10, "Buy bread");
    heap_insert(heap, 9, "Pay for the Internet");
    heap_insert(heap, 15, "Visit library5");
    heap_insert(heap, 15, "Visit library1");
    heap_insert(heap, 15, "Visit library2");
    heap_insert(heap, 15, "Visit library3");
    heap_insert(heap, 15, "Visit library4");


    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(heap);
    printf("Item: %d - %s \n", item.priority, item.value);


    heap_free(heap);

    return 0;
}
