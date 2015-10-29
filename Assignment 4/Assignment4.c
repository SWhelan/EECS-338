#include "main.h"
#include "linked_list.h"

void print_shared_info(struct shared_variables * shared){
    printf("--------------------------------------------\n");
    printf("pid: %d\n", getpid());
    printf("balance: %d\n", shared->balance);
    printf("wcount: %d\n", shared->wcount);
    printf("first list amount: %d\n", first_request_amount(shared->list));
    printf("--------------------------------------------\n");
}

int deposit(int amount){
    int semid, shmid;
	struct shared_variables *shared;

	semid = semget(SEMKEY, NUMBER_OF_SEMAPHORES, 0777);
	shmid = shmget(SHMKEY, 0, 0);
	shared = (struct shared_variables *)shmat(shmid, 0, 0);
    
    P(semid, SEMAPHORE_MUTEX);
    printf("I %d give: %d\n", getpid(), amount);
    shared->balance = shared->balance + amount;
    print_shared_info(shared);
    if(shared->wcount == 0){
        //No withdrawal requests
        V(semid, SEMAPHORE_MUTEX);
    } else if(first_request_amount(shared->list) > shared->balance){
        //Not enough balance for first waiting withdrawal request
        V(semid, SEMAPHORE_MUTEX);
    } else {
        // The mutex is released by the waiting withdrawal customer
        V(semid, SEMAPHORE_WLIST);
    }
    return EXIT_SUCCESS;
}

int withdrawal(int amount){
    int semid, shmid;
	struct shared_variables *shared;

	semid = semget(SEMKEY, NUMBER_OF_SEMAPHORES, 0777);
	shmid = shmget(SHMKEY, 0, 0);
	shared = (struct shared_variables *)shmat(shmid, 0, 0);
    
    P(semid, SEMAPHORE_MUTEX);
    printf("I %d want: %d\n", getpid(), amount);
    if(shared->wcount == 0 && shared->balance > amount){
        //withdraw
        shared->balance = shared->balance - amount;
        printf("I %d withdrew: %d\n", getpid(), amount);
        print_shared_info(shared);
        V(semid, SEMAPHORE_MUTEX);
    } else {
        //Can't withdrawal increment count
        shared->wcount = shared->wcount + 1;
        //and add amount to end of list
        add_end_of_list(shared->list, amount);
        printf("I %d can't withdraw %d and will wait.\n", getpid(), amount);
        print_shared_info(shared);
        //let depositors deposit or more withdrawalers wait
        V(semid, SEMAPHORE_MUTEX);
        //wait for a deposit to add enough money
        P(semid, SEMAPHORE_WLIST);
        printf("I %d got to withdraw %d.\n", getpid(), first_request_amount(shared->list));
        //withdraw
        shared->balance = shared->balance - first_request_amount(shared->list);
        //remove self from list
        delete_first_request(shared->list);
        //and count
        shared->wcount = shared->wcount - 1;
        print_shared_info(shared);
        if(shared->wcount > 1 && first_request_amount(shared->list) < shared->balance){
            //another withdrawal is waiting and can go
            V(semid, SEMAPHORE_WLIST);
        } else {
            //allow the depositor to go
            V(semid, SEMAPHORE_MUTEX);
        }
    }
    return EXIT_SUCCESS;
}

int main(){
    int semid;
    int shmid;
    unsigned short seminit[NUMBER_OF_SEMAPHORES];
	struct shared_variables * shared;
	union semun semctlarg;
    
    semid = semget(SEMKEY, NUMBER_OF_SEMAPHORES, 0777 | IPC_CREAT);
	seminit[SEMAPHORE_MUTEX]=1;
	seminit[SEMAPHORE_WLIST]=0;
	semctlarg.array = seminit;
	semctl(semid, NUMBER_OF_SEMAPHORES, SETALL, semctlarg);
    
    shmid = shmget(SHMKEY, sizeof(struct shared_variables), 0777 | IPC_CREAT);
	shared=(struct shared_variables *)shmat(shmid, 0, 0);
	shared->wcount = 0;
	shared->balance = 500;
	shared->list = (struct linked_list *)malloc(sizeof(struct linked_list));
    
    int i;
   	pid_t child_fpid;	
    int randVal;
    int amount;
    for(i = 0; i < 10; i++){
        randVal = rand() % 10;
        printf("FACTORY PROCESS INFO: Customer number %d will be created after ", i);
        printf("sleeping for %d.\n", randVal);
        sleep(randVal);
        child_fpid = fork();
        if(child_fpid < 0){
            // Handle if the fork() call failed.
            fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
            perror("An error occurred while executing fork.");
            return EXIT_FAILURE;
        } else if (child_fpid == 0){
            //Child process
            if(rand() % 2 == 0){
               amount = rand() % 10 * 25;
               printf("FACTORY PROCESS INFO: A deposit with %d dollars was created\n", amount);
              return deposit(amount);
            } else {
               amount = rand() % 10 * 10;
               printf("FACTORY PROCESS INFO: A withdraw for %d dollars was created\n", amount);
               return withdrawal(amount);
            }
        }
    }
    return EXIT_SUCCESS;
}