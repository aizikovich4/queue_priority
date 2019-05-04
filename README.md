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
1. Next defines: 
#define COUNT_WRITERS       1
#define COUNT_READERS       1
description minimum set of readers and writers threads. They may sets in different integer value (not negative) for emulate stress testing.
2. About High water mark"/"Low water mark": in this aplication heap-structure execute callback function for execute in different case and for interconnection with main code. 
- Queue send "Low water mark" event (by callbacks) when remove last element, and counter size equals to zero.
- Queue send "High water mark" event when counter of elements are equal to max size queue and next incoming elements will be ignored and not puts to queue. 
It is my thinks how it must work. Above events are not change any internal logic in this application, but look like logical that we must handles this events and such situation and make some actions - such as stop reading, writing, or may increase queue size (reallocation of internal bufffer are not supported), stop threads (or kill them).
3. Also have supports additional tools - handler user command from keyboard - it is made for testing of application in different condition - we may change count of read-write threads
#define MAX_ADDS_THREADS     10
Above define describe maximum counter of writes and reads threads (for every category - 10 reads && 10 writes).
How it use: 
```
  q- add new readers thread;
  w - add new writers thread;
  a - kill readers thread
  s - kill writers thread
```
(press "Enter" after every command)
It is my local stress-testing tools 8)

## it builds with GCC 5.4.0 on Ubuntu 16.04(x86), use pthread library and CMake (min. 2.8).
## How build:
Go to the directory with source
```
mkdir build
cd  build
cmake ..
make
```
For start:
```
./SperasoftPriorityQueue 
```
Example of output: 
```
Writer insert item: 5 - Task 5,  3030383424 
Queue: 0 5 2 1 0 
	 Reader(3059489600) get item: prior(5), Task 5, 3030383424 
Queue: 0 2 0 1 
	 Reader(3051096896) get item: prior(2), Task 2, 3076275008 
Writer insert item: 8 - Task 8,  3076275008
```
What we see above:  writer-thread  send element with priority 5, additional field - "Task5" and 3030383424(thread ID). That values do not have any hidden means, only for example.
After that we see "Queue: 0 5 2 1 0 " - it from reader-thread - it internal view of prioritet queue - they sorted by prioritet. Next reader-thread get element from queue with max priority. In output - we see current thread id, priority, and additional info. It is also for clarity, not means some usefull info.

If see many of "LOW_WATER_MARK received" - increase count of writers - press 'w' and 'Enter', or decrease count of readers - press 'a'
If see many of "HIGH_WATER_MARK received" - incread count of readers - press 'a' and 'Enter', or decrease count of writers - press 's'

