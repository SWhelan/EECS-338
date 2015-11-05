Sarah Whelan
slw96
11/5/2015

Posix Thread Programming

Readers and Writers with Starving Writers

The main method randomly sleeps between 0 and 4 seconds then spawns a reading or writing thread.
The type of thread is chosen randomly (with equal probability). 
The thread waits on posix semaphores appropriately before entering CS.
In CS each thread prints the current time, the thread id, the current thread status and the action.
Where the action is either reading or writing.

To run:
make
./Assignment5

Twenty threads are created in each run for convenience two runs have been recorded and included ine
run1.txt and run2.txt.