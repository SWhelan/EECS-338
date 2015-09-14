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
#include <unistd.h>

int main() {
    int i;
	pid_t child1_fpid;
	pid_t child2_fpid;
	pid_t child3_fpid;
	pid_t child4_fpid;	
	int termination_status;
	pid_t child_id;
	printf("Hello, I am the parent process. My process id is %d. Let's get started!\n", getpid());
    
    child1_fpid = fork();
    
    if(child1_fpid < 0){
	    perror("An error occured while executing fork.");
	    return EXIT_FAILURE;
    } else if (child1_fpid == 0){
        return child1();
    }
    
    sleep(2);
    child2_fpid = fork();
    
    if(child2_fpid < 0){
	    perror("An error occured while executing fork.");
	    return EXIT_FAILURE;
    } else if (child2_fpid == 0){
        sleep(1);
        return child2();
    }
    
    child3_fpid = fork();
    
    if(child3_fpid < 0){
	    perror("An error occured while executing fork.");
	    return EXIT_FAILURE;
    } else if (child3_fpid == 0){
        return child3();
    }
    
    child4_fpid = fork();
    
    if(child4_fpid < 0){
	    perror("An error occured while executing fork.");
	    return EXIT_FAILURE;
    } else if (child4_fpid == 0){
        return child4();
    }
	
	
	sleep(15);
	while((child_id = wait(&termination_status)) > 0){
    	printf("I the parent class lovingly waited for my child (with process id %d). They terminated with an exit status of %d.\n", child_id, termination_status);
    }
	
    return EXIT_SUCCESS;

}

int child1(){
    //this is the first child process
    printf("Hello, I am the first child! My process id is %d\n", getpid());
    printf("(n (n-2)) bionomial coefficient computations of integers n=2, 3, 10, start now!\n");
    return EXIT_SUCCESS;
}

int child2(){
    int i;
    for(i = 2; i < 11; i++){
        if(i % 2 == 0){
            printf("Child 2 process reporting in with %d choose %d: %d.\n", i, i-2, binomial_coefficient(i, i-2));
        } else {
            sleep(1);
        }
    }
    return EXIT_SUCCESS;
}

int child3(){
    int i;
    for(i = 2; i < 11; i++){
        if(i % 2 != 0){
            printf("Child 3 process reporting in with %d choose %d: %d.\n", i, i-2, binomial_coefficient(i, i-2));
        } else {
            sleep(1);
        } 
    }
    return EXIT_SUCCESS;
}

int child4(){
    sleep(10);
    printf("Child 4 process reporting with the results of ls -l.\n");
    execlp("ls", "ls", "-l", (char *) NULL);
    perror("Error occured while calling exec on ls -l.");
    return EXIT_FAILURE;
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
