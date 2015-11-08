/*
 * Sarah Whelan
 * slw96
 * 11/5/2015
 * 
 * Assignment 5
 * Posix Thread Programming
 * The Readers-Writers Problem with Starving Writers
 *
 * Runs on Linux
 * Compiles with gcc - see Makefile
 * No environment settings/paths required
 * libraries required - see import statements below
 * There are no other files required.
 *
 */

#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
 
#define READING 0
#define WRITING 1
#define SEMAPHORE_COUNT 2
#define THREAD_COUNT 20
  
typedef struct thread_common {
    int readcount;
    sem_t mutex;
    sem_t wrt;
} thread_common;

void reading(void * data);
void writing(void * data);
void print_status(int action);

int main(void){
    // Seed random number generator
    srand(time(NULL));
    
    // Thread variables
    int i;
    struct thread_common * shared;
    pthread_t ptids[THREAD_COUNT];
    int create_status;
    int join_status;
    int random_sleep;
    
    // Initialize thread data and array of thread ids
    shared = malloc(sizeof(*shared));
    if(shared == NULL){
        perror("Failed to malloc shared memory.");
        exit(EXIT_FAILURE);
    }
    shared->readcount = 0;
    
    // Initialize Semaphores
    if(sem_init(&shared->mutex, 0, 1) < 0) {
		perror("Failed to initialize mutex semaphore.\n");
		exit(EXIT_FAILURE);
	}
    
    if(sem_init(&shared->wrt, 0, 1) < 0) {
		perror("Failed to initialize wrt semaphore.\n");
		exit(EXIT_FAILURE);
	}
    
    // Create threads randomly in between sleeping randomly
    for(i = 0; i < THREAD_COUNT; i++){
        random_sleep = rand() % 5;
        printf("THREAD FACTORY INFO: Thread #%d will be created after sleeping for %d sec.\n", i, random_sleep);
        sleep(random_sleep);
        ptids[i] = i;
        if(rand() % 2 == 0){
            // new reading thread
            printf("THREAD FACTORY INFO: Thread #%d created to read.\n", i);
            create_status = pthread_create(&ptids[i], NULL, (void *)reading, shared);
            if(create_status == -1){
                perror("There was an error creating a reading thread.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            // new writing thread
            printf("THREAD FACTORY INFO: Thread #%d created to write.\n", i);
            create_status = pthread_create(&ptids[i], NULL, (void *)writing, shared);
            if(create_status == -1){
                perror("There was an error creating a writing thread.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    // Join the threads
    for(i = 0; i < THREAD_COUNT; i++){
        join_status = pthread_join(ptids[i], NULL);
        if(join_status != 0){
            perror("There was an error joining a thread.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // Free the shared memory and exit
    printf("Freeing shared thread memory and exiting.\n");
    free(shared);
    return EXIT_SUCCESS;
}

void sem_wait_helper(sem_t * sem){
    int sem_op_status;
    sem_op_status = sem_wait(sem);
    if(sem_op_status == -1){
        perror("There was an error waiting on the semaphore.");
        exit(EXIT_FAILURE);
    }
}

void sem_post_helper(sem_t * sem){
    int sem_op_status;
    sem_op_status = sem_post(sem);
    if(sem_op_status == -1){
        perror("There was an error signalling the semaphore.");
        exit(EXIT_FAILURE);
    }
}

// Reading thread
void reading(void * data){
    // Get shared thread data
    struct thread_common * shared;      
    shared = (thread_common *) data;
    
    // Wait on mutex
    sem_wait_helper(&shared->mutex);
    // Increase read count and possibly wait on wrt
    shared->readcount = shared->readcount + 1;
    if(shared->readcount == 1){
        sem_wait(&shared->wrt);
    }
    // Signal mutex
    sem_post_helper(&shared->mutex);

    // CS Reading
    print_status(READING);
    
    // Wait on mutex
    sem_wait_helper(&shared->mutex);
    shared->readcount = shared->readcount - 1;
    // If read count is now zero signal wrt
    if(shared->readcount == 0){
        sem_post_helper(&shared->wrt);
    }
    // Signal mutex
    sem_post_helper(&shared->mutex);
    // Exit
    pthread_exit((void*) data);
    return;
}

// Writing thread
void writing(void * data){
    // Get shared thread data
    struct thread_common * shared;      
    shared = (thread_common *) data;
    
    // Wait on the wrt semaphore
    sem_wait_helper(&shared->wrt);
    // CS Writing
    print_status(WRITING);
    // Signal the wrt semaphore
    sem_post_helper(&shared->wrt);
    // Exit
    pthread_exit((void*) data);
    return;
}

// Print status
void print_status(int action){
    // Time and thread state variables
    time_t current_time;
    int state_status;
    int state;
    char *state_desc;
    pthread_attr_t attr;
    char *action_desc;
    
    struct timespec req, rem;
    req.tv_sec = 3;
    req.tv_nsec = 5;
    
    // Get current state of the thread
    state_status = pthread_attr_getdetachstate(&attr, &state);
    if(state_status != 0){
        perror("There was an error getting the thread state.");
        return;
    }
    
    // Set string for current state
    if(state == PTHREAD_CREATE_DETACHED){
        state_desc = "detached";
    } else if (state == PTHREAD_CREATE_JOINABLE){
        state_desc = "joinable";
    }
    
    // Set string for action
    if(action == READING){
        action_desc = "reading";
    } else if(action == WRITING){
        action_desc = "writing";
    }
    
    // Print current time, thread id, thread state and action
    printf("-------------------------------------------------------------\n");
    // Get current time
    time(&current_time);
    printf("The current time is: %s", ctime(&current_time));
    printf("I thread, %ld, will sleep for 3 seconds while in my CS.\n", syscall(SYS_gettid));
    fflush(stdout);
    if(nanosleep(&req, &rem) < 0){
        perror("Failed nanosleep call.");
        exit(EXIT_FAILURE);
    }
    // Get current time
    time(&current_time);
    printf("I am awake and the current time is: %s", ctime(&current_time));
    printf("I thread, %ld, am in a %s state and I am %s!\n", syscall(SYS_gettid), state_desc, action_desc);
    printf("-------------------------------------------------------------\n");
    fflush(stdout);
    return;
}