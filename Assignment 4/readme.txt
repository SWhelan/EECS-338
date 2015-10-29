Assignment 4

Sarah Whelan
slw96
10/29/2015

Things to know about logistics of reading/running program:
- In the print out each process identifies themselves as their pid and say the amount they are trying to deposit/withdraw.
- In the print out if there isn't anything in the waiting list the first amount waiting is shown as -1.
- To run the program:
    make
    ./Assignment4
- The comments that are required by the grading policy are in main.h

Things to know about decisions in the code:
- The biggest implementation decision was the linked list I ended up determining for the way I wanted to do it every node had to be allocated in shared memory
- Each node has a different shared memory key and id and the nodes keep not links to other nodes but the next node's key so that the calling process can access each node
- The linked list itself also contains the key for the head and tail of the list (for convenience when adding to the end/not traversing)
- I saved the keys and in each access need to get the id with shmget and then the actual memory with shmat it may have been faster/more efficent to save the ids but the keys worked as well

Things to know about how I "randomized" customers/tested:
- The program forks 20 customers (sleeping for random amounts between 0 and 9 seconds in between each fork) and randomly assigns a value to deposit or withdraw.

Here is a copy of the code that sets the types/values:

            if(rand() % 10 > 7){
               amount = rand() % 10 * 400;
               printf("FACTORY PROCESS INFO: A deposit with %d dollars was created\n", amount);
              return deposit(amount);
            } else {
               amount = rand() % 10 * 150;
               printf("FACTORY PROCESS INFO: A withdraw for %d dollars was created\n", amount);
               return withdraw(amount);
            }
            
This ends up making more withdrawals than deposits but the deposits are larger. This shows
the linked list more than just a bunch of withdrawals that can always go.

Attached are two runs with 20 customers each with the above "randomness" settings (run1.txt and run2.txt).

The third run (run3.txt) is still 20 customers but with the following settings so that the list is probably never used. (The amounts are smaller but there are still more withdrawals.)

            if(rand() % 10 > 7){
               amount = rand() % 10;
               printf("FACTORY PROCESS INFO: A deposit with %d dollars was created\n", amount);
              return deposit(amount);
            } else {
               amount = rand() % 10;
               printf("FACTORY PROCESS INFO: A withdraw for %d dollars was created\n", amount);
               return withdraw(amount);
            }
            
Thank you for taking the time to grade all of these. Hope your day is going well.