/*
 * Sarah Whelan
 * slw96
 * 10/29/2015
 *
 * Runs on Ubuntu
 * Compiles with gcc - see Makefiles
 * No environment settings/paths required
 * libraries required - see import statments below
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEMKEY 77
#define SHMKEY 77

#define NUMBER_OF_SEMAPHORES 2
#define SEMAPHORE_MUTEX 0
#define SEMAPHORE_WLIST 1

typedef struct shared_variables {
    int wcount;
    int balance;
    struct linked_list * list;
} shared_variables;
 
// wait primitive
void P(int semid, int semaphore){
    struct sembuf psembuf;
    psembuf.sem_op = -1;
    psembuf.sem_flg = 0;
    psembuf.sem_num = semaphore;
    semop(semid,&psembuf,1);
    return;
}

// signal primitive
void V(int semid, int semaphore){
    struct sembuf vsembuf;
    vsembuf.sem_op = 1;
    vsembuf.sem_flg = 0;
    vsembuf.sem_num = semaphore;
    semop(semid,&vsembuf,1);
    return;
}

// required union for shared memory operations
union semun {
    int             val;            /* value for SETVAL */
    struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
    unsigned short  *array;         /* array for GETALL & SETALL */
    struct seminfo *__buf;          /* buffer for IPC_INFO */
};