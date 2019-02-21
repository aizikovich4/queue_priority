#include <stdio.h>
#include <stdlib.h>

struct heapitem {
    int priority;   /* priority of elemen */
    char *value;    /* data*/
};

struct heap {
    int maxsize; /* max heap size */
    int nitems; /* count of elements ih heap */
    struct heapitem *items;
};

struct heap *heap_create(int maxsize)
{
    struct heap *h;
    h = malloc(sizeof(*h));
    if (h != NULL)
    {
        h->maxsize = maxsize;
        h->nitems = 0;
        h->items = malloc(sizeof(struct heapitem) *
        (maxsize + 1));

        if (h->items == NULL)
        {
            free(h);
            return NULL;
        }
    }

    return h;
}

void heap_free(struct heap *h)
{
    free(h->items);
    free(h);
}

void heap_swap(struct heapitem *a, struct heapitem *b)
{
    struct heapitem temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

struct heapitem heap_max(struct heap *h)
{
    struct heapitem erritem = {-1, NULL};
    if (h->nitems > 0)
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

int heap_insert(struct heap *h, int priority, char *value)
{
    int i;
    if (h->nitems >= h->maxsize)
    {
        fprintf(stderr,
        "heap: Heap overflow.\n");
        return -1;
    }
    h->nitems++;
    h->items[h->nitems].priority = priority;
    h->items[h->nitems].value = value;
    /* Продвигаем элемент вверх */
    for (i = h->nitems;    i > 1 && h->items[i].priority > h->items[i / 2].priority;  i = i / 2)
    {
        heap_swap(&h->items[i], &h->items[i / 2]);
    }
    return 0;
}

struct heapitem heap_removemax(struct heap *h)
{
    int k, n, j;
    heap_swap(&h->items[1],
    &h->items[h->nitems]);
    for (k = 1, n = h->nitems - 1; 2 * k <= n;  k = j)
    {
        j = 2 * k;
        if (j < n && h->items[j].priority <  h->items[j + 1].priority)
        {
            j++;
        }
        if (h->items[k].priority >= h->items[j].priority)
        {
            break;
        }
        heap_swap(&h->items[k], &h->items[j]);
    }
    return h->items[h->nitems--];
}



int main(void)
{
    struct heap *h;
    struct heapitem item;
    h = heap_create(20);
    heap_insert(h, 10, "Buy bread");
    heap_insert(h, 9, "Pay for the Internet");
    heap_insert(h, 15, "Visit library5");
    heap_insert(h, 15, "Visit library1");
    heap_insert(h, 15, "Visit library2");
    heap_insert(h, 15, "Visit library3");
    heap_insert(h, 15, "Visit library4");


    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);
    item = heap_removemax(h);
    printf("Item: %d - %s \n", item.priority, item.value);


    heap_free(h);

    return 0;
}

