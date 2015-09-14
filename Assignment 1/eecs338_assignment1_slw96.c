/*
 * Sarah Whelan
 * slw96
 * 
 * EECS 338
 * Assignment 1
 *
 */

#include <stdio.h>
#include <stdlib.h>
int main() {
	pid_t child1_fpid;	
	int termination_status;
	pid_t child1_id;
	printf("Hello, I am the parent process. My process id is %d. Let's get started!\n", getpid());
    
	child1_fpid = fork();
    
	if(child1_fpid < 0){
		perror("An error occured while executing fork.");
		return EXIT_FAILURE;
	} else if (child1_fpid == 0){
		//this is the first child process
		printf("Hello, I am the first child! My process id is %d\n", getpid());
		printf("(n (n-2)) bionomial coefficient computations of integers n=2, 3, 10, start now!\n");
		return EXIT_SUCCESS;
	} else {
	    //this is still the parent class
	    child1_id = wait(&termination_status);
	    printf("I the parent class lovingly waited for my child (with process id %d). They terminated with an exit status of %d.\n", child1_id, termination_status);
	}

return EXIT_SUCCESS;

}		
