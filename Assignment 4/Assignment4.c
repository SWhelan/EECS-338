#include "main.h"
#include "linked_list.h"

void print_shared_info(struct shared_variables * shared){
    printf("-----------------------------\n");
    printf("Shared memory info update:\n");
    printf("pid: %d\n", getpid());
    printf("balance: %d\n", shared->balance);
    printf("wcount: %d\n", shared->wcount);
    printf("first list amount: %d\n", first_request_amount(shared->list_key));
    printf("-----------------------------\n");
}

int deposit(int amount){
    // Gain access to shared memory
    int semid;
    int shmid;
	struct shared_variables *shared;
	semid = semget(SEMKEY, NUMBER_OF_SEMAPHORES, 0777);
    if(semid == -1){
        printf("There was an error getting the semaphores. Aborting.\n");
        return EXIT_FAILURE;
    }
	shmid = shmget(SHMKEY, 0, 0);
    if(shmid == -1){
        printf("There was an error creating the shared memory. Aborting.\n");
        return EXIT_FAILURE;
    }
	shared = (struct shared_variables *)shmat(shmid, 0, 0);
    if(shared == (void *) -1){
        printf("There was an error creating the shared memory. Aborting.\n");
        return EXIT_FAILURE;
    }
    
    //Wait for mutex 
    P(semid, SEMAPHORE_MUTEX);
    
    //Deposit immediately 
    printf("I, %d, give: %d\n", getpid(), amount);
    shared->balance = shared->balance + amount;
    print_shared_info(shared);
    if(shared->wcount == 0){
        //No withdrawal requests
        V(semid, SEMAPHORE_MUTEX);
    } else {
        if(first_request_amount(shared->list_key) > shared->balance){
            //Not enough balance for first waiting withdrawal request
            V(semid, SEMAPHORE_MUTEX);
        } else {
            // Signal a waiting withdraw. The mutex is released by the waiting withdrawal customer
            V(semid, SEMAPHORE_WLIST);
        }
    }
    return EXIT_SUCCESS;
}

int withdraw(int amount){
    // Gain access to shared memory
    int semid;
    int shmid;
	struct shared_variables *shared;
	semid = semget(SEMKEY, NUMBER_OF_SEMAPHORES, 0777);
    if(semid == -1){
        printf("There was an error getting the semaphores. Aborting.\n");
        return EXIT_FAILURE;
    }
	shmid = shmget(SHMKEY, 0, 0);
    if(shmid == -1){
        printf("There was an error creating the shared memory. Aborting.\n");
        return EXIT_FAILURE;
    }
	shared = (struct shared_variables *)shmat(shmid, 0, 0);
    if(shared == (void *) -1){
        printf("There was an error creating the shared memory. Aborting.\n");
        return EXIT_FAILURE;
    }
    
    P(semid, SEMAPHORE_MUTEX);
    printf("I, %d, want: %d\n", getpid(), amount);
    if(shared->wcount == 0 && shared->balance >= amount){
        //withdraw
        shared->balance = shared->balance - amount;
        printf("I, %d, withdrew: %d\n", getpid(), amount);
        print_shared_info(shared);
        V(semid, SEMAPHORE_MUTEX);
    } else {
        //Can't withdrawal wait until its possible
        shared->wcount = shared->wcount + 1;
        add_end_of_list(shared->list_key, amount);
        printf("I, %d, can't withdraw %d and will wait.\n", getpid(), amount);
        print_shared_info(shared);
        V(semid, SEMAPHORE_MUTEX);
        P(semid, SEMAPHORE_WLIST);
        printf("I, %d, got to withdraw %d.\n", getpid(), first_request_amount(shared->list_key));
        //withdraw
        shared->balance = shared->balance - first_request_amount(shared->list_key);
        //remove self from list
        delete_first_request(shared->list_key);
        //and decrement count
        shared->wcount = shared->wcount - 1;
        print_shared_info(shared);
        
        if(shared->wcount >= 1 && first_request_amount(shared->list_key) < shared->balance){
            //another withdrawal is waiting and can go
            V(semid, SEMAPHORE_WLIST);
        } else {
            //The next withdrawal if there is one can't go or there isn't a next withdrawal
            V(semid, SEMAPHORE_MUTEX);
        }
    }
    return EXIT_SUCCESS;
}

int main(){
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Shared Memory Variables
    int semid;
    int shmid;
    int list_id;
    struct linked_list * list;
    unsigned short seminit[NUMBER_OF_SEMAPHORES];
	struct shared_variables * shared;
	union semun semctlarg;
    
    // Process generation and testing variables
    int i;
   	pid_t child_fpid;
    int randVal;
    int amount;
    
    pid_t pids[NUMBER_OF_CUSTOMERS];
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        pids[i] = 0;
    }
    
    // Shared Memory Initialization
    semid = semget(SEMKEY, NUMBER_OF_SEMAPHORES, 0777 | IPC_CREAT);
	seminit[SEMAPHORE_MUTEX]=1;
	seminit[SEMAPHORE_WLIST]=0;
	semctlarg.array = seminit;
	semctl(semid, NUMBER_OF_SEMAPHORES, SETALL, semctlarg);
    
    shmid = shmget(SHMKEY, sizeof(struct shared_variables), 0777 | IPC_CREAT);
	shared=(struct shared_variables *)shmat(shmid, 0, 0);
	shared->wcount = 0;
	shared->balance = 500;
	shared->list_key = SHMKEY + 1;
    
    // Initialize the linked list in shared memory
    list_id = shmget(shared->list_key, sizeof(struct linked_list), 0777 | IPC_CREAT);
    list = (struct linked_list *)shmat(list_id, 0, 0);
    list->tail_key = -1;
    list->head_key = -1;
    list->self_key = shared->list_key;
    list->size = 0;
    
    // Process generation 
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        randVal = rand() % 10;
        printf("FACTORY PROCESS INFO: Customer #%d will be created after sleeping for %d sec.\n", i, randVal);
        sleep(randVal);
        child_fpid = fork();
        pids[i] = child_fpid;
        if(child_fpid < 0){
            // Handle if the fork() call failed.
            fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
            perror("An error occurred while executing fork.");
            return EXIT_FAILURE;
        } else if (child_fpid == 0){
            //Child process
            if(rand() % 10 > 7){
               amount = rand() % 10 * 400;
               printf("FACTORY PROCESS INFO: A deposit with %d dollars was created\n", amount);
              return deposit(amount);
            } else {
               amount = rand() % 10 * 150;
               printf("FACTORY PROCESS INFO: A withdraw for %d dollars was created\n", amount);
               return withdraw(amount);
            }
            break;
        }
    }
    
    /*
     * Can't wait on processes as they may be stuck at semaphores when this terminates
     * ie a withdrawal that was never able to withdraw
     */
    sleep(10);
    
    // Kill the processes
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        if(pids[i] != 0){
            kill(pids[i], SIGKILL);
            wait(0);
        }
    }
    printf("Forked Processes terminated. Releasing shared memory.\n");
    
    //Release the semaphores
    semctl(semid, NUMBER_OF_SEMAPHORES, IPC_RMID, 0);
    
    //Release the shared memory
    release_linked_list(shared->list_key);
    shmctl(shmid, IPC_RMID, 0);
    printf("Thank you for banking with System 5 Semaphores. Have a nice day!");
    return EXIT_SUCCESS;
}