#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <pthread.h>
#include<unistd.h>

#include "heap.h"

#define COUNT_WRITERS   2
#define COUNT_READERS   2

void events_queue_handler(int flag);
pthread_t tid_writer[COUNT_WRITERS];
pthread_t tid_reader[COUNT_READERS];
static pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;
struct Heap *heap;

void* thread_writers(void *par);
void* thread_reader(void *par);

void run_threads();

void* thread_writers(void *par)
{
    unsigned int i=0;
     time_t t;
    char *mas[]= {"Task 1","Task 2","Task 3","Task 4","Task 5"};

    printf("start writer thread \n");
    srand ( time(&t) );

    while(1)
    {
        pthread_mutex_lock(&mutex_thread);
        for(i=0; i<5; ++i)
        {
            int r= rand() % 20;
            printf("Writer insert item: %d - %s,  %lu \n",r, mas[i], pthread_self());
            heap_insert(heap, r, mas[i],pthread_self());

        }
        pthread_mutex_unlock(&mutex_thread);
        usleep(400);
    }

}

void* thread_reader(void *par)
{
    struct Heap_item item;

    printf("start reader thread \n");
    while(1)
    {
        pthread_mutex_lock(&mutex_thread);
        if(heap_size(heap))
            heap_display(heap);
        if(heap_removemax(heap, &item))   //get size ant removemax at once, else - race condition
        {
            printf("\t Reader(%lu) get item: %d - %s,  %lu \n",pthread_self(), item.priority, item.value, item.thread_id);
            usleep(40000);
        }
        pthread_mutex_unlock(&mutex_thread);
    }
}



void events_queue_handler(int flag)
{
    switch (flag) {
    case LOW_WATER_MARK:
        printf("LOW_WATER_MARK received \n");
        break;
    case HIGH_WATER_MARK:
        printf("HIGH_WATER_MARK received \n");
        break;
    default:
        break;
    }


}

int main(void)
{

    heap = heap_create(100, events_queue_handler);
    if(pthread_mutex_init(&mutex_thread, NULL))
        exit(2);
    run_threads();

    //printf("Max element with priority now: %d, %s \n",heap_max(heap).priority, heap_max(heap).value);

    while(1)
    {

    }
    heap_free(heap);

    return 0;
}

void run_threads()
{
    int err=0, i=0;
    for(i=0; i<sizeof(tid_writer)/sizeof(tid_writer[0]); ++i)
    {
        if( err=pthread_create(&(tid_writer[i]), NULL, &thread_writers, NULL))  printf("can't create thread :[%s]", strerror(err));
        pthread_detach(tid_writer[i]);
    }

    for(i=0; i<sizeof(tid_reader)/sizeof(tid_reader[0]); ++i)
    {
        if( err=pthread_create(&tid_reader[i], NULL, &thread_reader, NULL))     printf("can't create thread :[%s]", strerror(err));
        pthread_detach(tid_reader[i]);
    }
}

