/*
 * Sarah Whelan
 * slw96
 * 10/29/2015
 *
 * Runs on Linux - #102~prescise1-Ubuntu SMP
 * Compiles with gcc - see Makefile
 * No environment settings/paths required
 * libraries required - see import statements below
 * no other involved files include anything else
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define SEMKEY 77
#define SHMKEY 96

#define NUMBER_OF_SEMAPHORES 2
#define SEMAPHORE_MUTEX 0
#define SEMAPHORE_WLIST 1

#define NUMBER_OF_CUSTOMERS 20

// Shared variables
typedef struct shared_variables {
    int wcount;
    int balance;
    int list_key;
} shared_variables;
 
// Wait
void P(int semid, int semaphore){
    struct sembuf psembuf;
    psembuf.sem_op = -1;
    psembuf.sem_flg = 0;
    psembuf.sem_num = semaphore;
    semop(semid,&psembuf,1);
    return;
}

// Signal
void V(int semid, int semaphore){
    struct sembuf vsembuf;
    vsembuf.sem_op = 1;
    vsembuf.sem_flg = 0;
    vsembuf.sem_num = semaphore;
    semop(semid,&vsembuf,1);
    return;
}

// Required union for shared memory operations
union semun {
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    unsigned short *array; /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
};