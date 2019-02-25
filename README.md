# queue_priority
Queue with priorities  (test task)
my testing task for Sperasoft
Task 1: Need to design and implement a priority queue in C, and provide a test application which demonstrates that the queue is working.
A few requirements:
1. Priority values are integers
2. Priority queues should be thread-safe
3. Priority queues should generate "High water mark"/"Low water mark" events.
4. Test application should create at least two threads writing into the queue and at least two threads reading from the queue.
5. For anything which isn't mentioned in these requirements, you can make a choice yourself.
6. Test application should come with some instructions for compiling it.


# Review of project:
How application look inside:
1) Next defines: 
#define COUNT_WRITERS       1
#define COUNT_READERS       1
description minimum set of readers and writers threads. They may sets in different integer value (not negative) for emulate stress testing.
2) about High water mark"/"Low water mark": in this aplication heap-structure execute callback function for execute in different case and for interconnection with main code. 
Queue send "Low water mark" event (by callbacks) when remove last element, and counter size equals to zero.
Queue send "High water mark" event when counter of elements are equal to max size queue and next incoming elements will be ignored and not puts to queue. 
It is my thinks how it must work. Above events are not change any internal logic in this application, but look like logical that we must handles this events and such situation and make some actions - such as stop reading, writing, or may increase queue size (reallocation of internal bufffer are not supported), stop threads (or kill them).
3) Also have supports additional tools - handler user command from keyboard - it is made for testing of application in different condition - we may change count of read-write threads
#define MAX_ADDS_THREADS     10
Above define describe maximum counter of writes and reads threads (for every category - 10 reads && 10 writes).
How it use: 
  q- add new readers thread;
  w - add new writers thread;
  a - kill readers thread
  s - kill writers thread
(press "Enter" after every command)
It is my local stress-testing tools 8)



## it builds with GCC 5.4.0 on Ubuntu 16.04(x86), use pthread library and CMake (min. 2.8).
