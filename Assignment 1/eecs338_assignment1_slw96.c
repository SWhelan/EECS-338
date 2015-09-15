/*
 * Sarah Whelan
 * slw96
 * 
 * EECS 338
 * Assignment 1
 *
 * 9/17/2015
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#define LOOP_ITERATIONS 0x7fffffff

int main() {
    int i;
	pid_t child_fpid;	
	int termination_status;
	pid_t child_id; 
	printf("I am the parent process. My process id is %d.\n", getpid());
    print_ids();
    for(i = 1; i < 5; i++){
        child_fpid = fork();
    
        if(child_fpid < 0){
	        perror("An error occured while executing fork.");
	        return EXIT_FAILURE;
        } else if (child_fpid == 0){
            print_ids();
            sleep(3);
            if(i == 1){
                return child1();
            } else if(i == 2){
                return child2();
            } else if(i == 3){
                return child3();
            } else {
                return child4();
            }
        }
    }
	
	
	sleep(30);
	while((child_id = wait(&termination_status)) > 0){
    	printf("A child process with id %d terminated with an exit status of %d.\n", child_id, termination_status);
    }
	
	loop_and_print_times();
	
    return EXIT_SUCCESS;

}	

int child1(){
    //this is the first child process
    printf("(n (n-2)) bionomial coefficient computations of integers n=2, 3, 10, start now!\n");
    loop_and_print_times();
    return EXIT_SUCCESS;
}

int child2(){
    sleep(2);
    binomial_loop(2);
}

int child3(){
    sleep(3);
    binomial_loop(3);
}

int child4(){
    sleep(15);
    printf("Process (pid %d) will loop for a while and print how long it has taken then will run ls -l.\n", getpid());
    loop_and_print_times();
    printf("Process (pid %d) executing ls -l:\n", getpid());
    execlp("ls", "ls", "-l", (char *) NULL);
    perror("Error occured while calling exec on ls -l.");
    return EXIT_FAILURE;
}

int print_ids(){
    char * username;    
    username = (char *) cuserid((int*) NULL);
    if(username == NULL){
        perror("There was an error getting the username.");
        return EXIT_FAILURE;
    }
    printf("The current process id: %d\n", getpid());
    printf("The username: %s\n", username);
    printf("The real user id: %d\n", getuid());
    printf("The effective user id: %d\n", geteuid());
    printf("The group id: %d\n", getgid());
    print_seperator();
    return EXIT_SUCCESS;
}

int loop_and_print_times(){
    int i;
    time_t current_time;
    struct rusage usage;
    i = 0;
    while(i < 10000){
        i++;
    }
    
    if(getrusage(RUSAGE_SELF, &usage) < 0){
        perror("There was an error getting the system and user times for process.");
        return errno;
    }
    
    time(&current_time);
    printf("The current process with pid %d is about to terminate.\n", getpid());
    printf("The current time is: %s", ctime(&current_time));
    printf("The user CPU time is: %d\n", (int)usage.ru_utime.tv_usec);
    printf("The system CPU time is: %d\n", (int)usage.ru_stime.tv_usec);
    print_seperator();
    return EXIT_SUCCESS;
}

int binomial_loop(int start){
    int i;
    for(i = start; i < 11; i = i + 2){
        printf("Process (pid %d) calculated binomial coefficient %d with n = %d and k = %d.\n", getpid(), binomial_coefficient(i, i-2), i, i-2);
        sleep(2);
    }
    if(start == 2){
        printf("Finished finding binomial coefficients.\n");
        print_seperator();
    }
    if(start == 3){
        sleep(2);
    }
    loop_and_print_times();
    return EXIT_SUCCESS;
}

//Get the binomial coefficient - equivalent to n choose k
int binomial_coefficient(int n, int k){
    return factorial(n)/(factorial(k)*factorial(n-k));
}

//Get the factorial of an integer must be greater than or equal to 0
int factorial(int n){
    if(n < 2){
        return 1;
    } else {
        return n * factorial(n-1);
    }
}

int print_seperator(){
    printf("======================================\n");
}
