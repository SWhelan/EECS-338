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

// The largest 32 bit integer.
#define LOOP_ITERATIONS 0x7fffffff

int main() {
    int i;
	pid_t child_fpid;	
	int termination_status;
	pid_t child_id;
	
	// Introduce the parent process and print the relevant ids.
	printf("I am the parent process. My process id is %d.\n", getpid());
    print_ids();
    
    // Fork the four children processes.
    for(i = 1; i < 5; i++){
    
        child_fpid = fork();
    
        if(child_fpid < 0){
            // Handle if the fork() call failed.
	        perror("An error occurred while executing fork.");
	        return EXIT_FAILURE;
        } else if (child_fpid == 0){
            // The fork was successful and now the child process is executing.
            
            // Print the relevant ids for the child process.
            print_ids();
            // Give each child time to print ids before forking the next child
            sleep(3);
            
            // Call the appropriate method for each child
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
	
	// Wait for all four children to terminate.
	sleep(30);
	
	// Indicate a child process has terminated.
	while((child_id = wait(&termination_status)) > 0){
    	printf("A child process with id %d terminated with an exit status of %d.\n", child_id, termination_status);
    }
	
	// Print how long the parent process has been running.
	loop_and_print_times();
	
    return EXIT_SUCCESS;

}	

// The first child process.
int child1(){
    // Prints the introduction message.
    printf("(n (n-2)) binomial coefficient computations of integers n=2, 3, 10, start now!\n");
    
    // Prints usage time.
    loop_and_print_times();
    
    return EXIT_SUCCESS;
}

// The second child process.
int child2(){
    // Waits a little bit to start for simpler synchronization.
    sleep(2);
    
    // Prints binomial coefficients starting at 2 going up by 2 until 10.
    binomial_loop(2);
    
    return EXIT_SUCCESS;
}

// The third child process.
int child3(){
    // Waits a little longer than the second child for simpler synchronization.
    sleep(3);
    
    // Prints binomial coefficients starting at 3 going up by 2 until 9
    binomial_loop(3);
    
    return EXIT_SUCCESS;
}

// The fourth child process.
int child4(){
    // Waits until all binomial coefficients are printed.
    sleep(15);
    
    // Prints time statistics
    printf("Process (pid %d) will loop for a while and print how long it has taken then will run ls -l.\n", getpid());
    loop_and_print_times();
    
    // Executes the command "ls -l"
    printf("Process (pid %d) executing ls -l:\n", getpid());
    execlp("ls", "ls", "-l", (char *) NULL);
    
    // If exec returns to the calling process there was an error running the command
    perror("Error occurred while calling exec on ls -l.");
    return EXIT_FAILURE;
}

// Prints the pid, username, real and effective user ids, and group id.
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
    print_separator();
    return EXIT_SUCCESS;
}

// Loops for a while to accumulate CPU time then prints process time statistics.
int loop_and_print_times(){
    int i;
    time_t current_time;
    struct rusage usage;
    
    // Loop for a while.
    i = 0;
    while(i < 10000){
        i++;
    }
    
    // Get CPU time information about current process.
    if(getrusage(RUSAGE_SELF, &usage) < 0){
        // Handle possible error while getting information.
        perror("There was an error getting the system and user times for process.");
        return errno;
    }
    
    // Get the number of seconds since the epoch.
    time(&current_time);
    
    // Print information.
    printf("The current process with pid %d is about to terminate.\n", getpid());
    printf("The current time is: %s", ctime(&current_time));
    printf("The user CPU time is: %d\n", (int)usage.ru_utime.tv_usec);
    printf("The system CPU time is: %d\n", (int)usage.ru_stime.tv_usec);
    print_separator();
    
    return EXIT_SUCCESS;
}

// Prints out up to 5 binomial coefficients in increments of 2. Starting at the start argument.
int binomial_loop(int start){
    int i;
    for(i = start; i < 11; i = i + 2){
        printf("Process (pid %d) calculated binomial coefficient %d with n = %d and k = %d.\n", getpid(), binomial_coefficient(i, i-2), i, i-2);
        sleep(2);
    }
    if(start == 2){
        printf("Finished finding binomial coefficients.\n");
        print_separator();
    }
    if(start == 3){
        sleep(2);
    }
    loop_and_print_times();
    return EXIT_SUCCESS;
}

// Get the binomial coefficient - equivalent to n choose k.
int binomial_coefficient(int n, int k){
    return factorial(n)/(factorial(k)*factorial(n-k));
}

// Get the factorial of an integer must be greater than or equal to 0.
int factorial(int n){
    if(n < 2){
        return 1;
    } else {
        return n * factorial(n-1);
    }
}

// Print a convenient separator.
int print_separator(){
    printf("======================================\n");
}
