#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <pthread.h>
#include <time.h>
#include<unistd.h>

#include "heap.h"

#define COUNT_WRITERS   1
#define COUNT_READERS   1

#define MAX_ADDS_THREADS     5

void events_queue_handler(int flag);
void* thread_writers(void *par);
void* thread_reader(void *par);
void* user_console_thread(void *par);
static void run_threads();


pthread_t tid_writer[COUNT_WRITERS];
pthread_t tid_reader[COUNT_READERS];
pthread_t user_console;
static pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
struct Heap *heap;
static unsigned long time_read  = 250000;
static unsigned long time_write = 250000;
int err;
static char *mas[]= {"Task 0", "Task 1", "Task 2", "Task 3", "Task 4", "Task 5", "Task 6", "Task 7", "Task 8", "Task 9", "Task 10"};

void *thread_writers(void *par)
{
     time_t t;

    printf("start writer thread \n");
    srand ( time(&t) );

    while(1)
    {
        unsigned int r= rand() % 11; //write random count of items to queue
        if(!heap_insert(heap, r, mas[r],pthread_self()))
        {
            printf("Writer insert item: %d - %s,  %lu \n",r, mas[r], pthread_self());
            //heap_display(heap);
        }
        usleep(time_write); //sleep at this moment, for exclude queue overflow
    }

}

void* thread_reader(void *par)
{
    struct Heap_item item;

    printf("start reader thread \n");
    while(1)
    {
        if(heap_size(heap))
            heap_display(heap);
        if(heap_removemax(heap, &item))   //get size ant removemax at once, else - race condition
        {
            printf("\t Reader(%lu) get item: %d - %s,  %lu \n",pthread_self(), item.priority, item.value, item.thread_id);
            usleep(time_read); //emulate hard work of processor on new element from queue
        }
    }
}


void events_queue_handler(int flag)         //it custom handler events from queue. All necessary and additional action we may do in this place.
{
    switch (flag) {
        case LOW_WATER_MARK:
              //printf("LOW_WATER_MARK received \n");
            break;
        case HIGH_WATER_MARK:
              //printf("HIGH_WATER_MARK received \n");
            break;
        default:
            break;
    }
}

void run_threads()
{
    int i=0;
    err = 0;
    //writers threads start
    for(i=0; i<sizeof(tid_writer)/sizeof(tid_writer[0]); ++i)
    {
        if( err=pthread_create(&tid_writer[i], NULL, &thread_writers, NULL))
            printf("can't create thread :[%s]", strerror(err));
        pthread_detach(tid_writer[i]);
    }
    //readers threads start
    for(i=0; i<sizeof(tid_reader)/sizeof(tid_reader[0]); ++i)
    {
        if( err=pthread_create(&tid_reader[i], NULL, &thread_reader, NULL))
            printf("can't create thread :[%s]", strerror(err));
        pthread_detach(tid_reader[i]);
    }

    //user console thread start
    if( err=pthread_create(&user_console, NULL, &user_console_thread, NULL))
        printf("can't create thread :[%s]", strerror(err));
    pthread_detach(user_console);
}

void* user_console_thread(void *par)
{
    int user_command;
    int counter_writers = 0, counter_readers = 0;
    pthread_t array_writers[MAX_ADDS_THREADS+1]={};
    pthread_t array_readers[MAX_ADDS_THREADS+1]={};

    //help:
    //q- add new readers thread;
    //w - add new writers thread;
    //a - kill readers thread
    //s - kill writers thread

    while (1)
    {
        user_command = getchar();
        if(user_command != EOF)
        {
            switch (user_command)
            {
            case 'q':
                if(counter_readers < MAX_ADDS_THREADS)
                {
                    if( err=pthread_create(&array_readers[counter_readers++], NULL, &thread_reader, NULL))
                        printf("can't create thread :[%s]", strerror(err));
                    //pthread_detach(array_readers[counter_readers]);
                }
                break;
            case 'w':
                if(counter_writers < MAX_ADDS_THREADS)
                {
                    if( err=pthread_create(&array_writers[counter_writers++], NULL, &thread_writers, NULL))
                        printf("can't create thread :[%s]", strerror(err));
                   // pthread_detach(array_writers[counter_writers]);
                }
                break;
            case 'a':
                if(counter_readers >= 0)
                {
                    pthread_cancel(array_readers[counter_readers]);  //we can always set wrong canceling to thread whom unexist...
                    if(--counter_readers < 0)
                    {
                        printf("not more additional readers thread \n");
                        counter_readers = 0;
                    }
                }
                break;
            case 's':
                if(counter_writers >= 0)
                {
                    pthread_cancel(array_writers[counter_writers]);  //we can always set wrong canceling to thread whom unexist...
                    if(--counter_writers < 0)
                    {
                        printf("not more additional writers thread \n");
                        counter_writers = 0;
                    }
                }
                break;
            default:
                break;
            }
        }

    }
}
int main(void)
{

    heap = heap_create(150, events_queue_handler);
    if(pthread_mutex_init(&mutex_thread, NULL))
        exit(2);

    run_threads();

    //printf("Max element with priority now: %d, %s \n",heap_max(heap).priority, heap_max(heap).value);

    while(1)
    {

    }
    heap_free(heap);
    pthread_mutex_destroy(&mutex_thread);
     pthread_cond_destroy(&cond);
    return 0;
}



