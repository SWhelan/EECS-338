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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

// The largest 32 bit integer.
#define LOOP_ITERATIONS 0x7fffffff

// How long in seconds it takes to loop to the largest 32 bit integer
#define LOOP_SLEEP_TIME 45

// How long in seconds it takes to print ids
#define PRINT_IDS_SLEEP_TIME 5

int main() {
    int i;
    const int number_of_children = 4;
	pid_t child_fpid;	
	int termination_status;
	pid_t child_id;
	
	// Introduce the parent process and print the relevant ids.
	printf("I am the parent process. My process id is %d.\n", getpid());
    print_ids();
    
    // Fork the four children processes.
    for(i = 0; i < number_of_children; i++){
    
        child_fpid = fork();
    
        if(child_fpid < 0){
            // Handle if the fork() call failed.
            fprintf(stderr, "errno %d: %d\n", errno, strerror(errno));
	        perror("An error occurred while executing fork.");
	        return EXIT_FAILURE;
        } else if (child_fpid == 0){
            // The fork was successful and now the child process is executing.
            
            // Print the relevant ids for the child process.
            print_ids();
            
            // Give each child time to print ids before forking the next child
            sleep(PRINT_IDS_SLEEP_TIME);
            
            // Call the appropriate method for each child
            if(i == 0){
                return child1();
            } else if(i == 1){
                return child2();
            } else if(i == 2){
                return child3();
            } else {
                return child4();
            }
        }
    }
	
	// Wait for all the children to terminate and print a message for each one.
	while((child_id = wait(&termination_status)) > 0){
    	printf("My child process with pid %d terminated with an exit status of %d.\n", child_id, WEXITSTATUS(termination_status));
    	print_separator();
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
    sleep(LOOP_SLEEP_TIME);
    
    // Prints binomial coefficients starting at 2 going up by 2 until 10.
    binomial_loop(2);
    
    // Print times
    loop_and_print_times();
    
    return EXIT_SUCCESS;
}

// The third child process.
int child3(){
    // Waits a little longer than the second child for simpler synchronization.
    sleep(LOOP_SLEEP_TIME + 1);
    
    // Prints binomial coefficients starting at 3 going up by 2 until 9
    binomial_loop(3);
    
    // Wait for Child 2 to loop and terminate.
    sleep(LOOP_SLEEP_TIME);
    
    // Print times
    loop_and_print_times();
    
    return EXIT_SUCCESS;
}

// The fourth child process.
int child4(){
    const char * ls_command = "ls";
    char * const ls_arguments [] = {"ls", "-l", NULL};
    
    // Waits until all binomial coefficients are printed.
    sleep(LOOP_SLEEP_TIME * 3);
    
    // Prints time statistics
    printf("Process (pid %d) will loop for a while and print how long it has taken then will run ls -l.\n", getpid());
    loop_and_print_times();
    
    // Executes the command "ls -l"
    printf("Process (pid %d) executing ls -l:\n", getpid());
    execvp(ls_command, ls_arguments);
    
    // If exec returns to the calling process there was an error running the command
    perror("Error occurred while calling exec on ls -l.");
    return EXIT_FAILURE;
}

// Prints the pid, username, real and effective user ids, and group id.
int print_ids(){
    char * username;
    printf("The current process id: %d\n", getpid());
    
    username = (char *) cuserid((int*) NULL);
    if(username == NULL){
        perror("There was an error getting the username.");
        return EXIT_FAILURE;
    } else {
        printf("The username: %s\n", username);
    }
    
    // The man pages indicate the following system calls are always successful.
    printf("The real user id: %d\n", getuid());
    printf("The effective user id: %d\n", geteuid());
    printf("The group id: %d\n", getgid());
    printf("The effective group id: %d\n", getegid());
    print_separator();
    return EXIT_SUCCESS;
}

// Loops for a while to accumulate CPU time then prints process time statistics.
int loop_and_print_times(){
    int i;
    time_t current_time;
    struct tms time_info;
    
    // Get the number of seconds since the epoch.
    time(&current_time);
    
    // Print something so it is clear something is happening.
    printf("The current process with pid %d is about to loop and terminate.\n", getpid());
    printf("The current time is: %s", ctime(&current_time));
    
    // Loop for a while.
    i = 0;
    while(i < LOOP_ITERATIONS){
        i++;
    }
    
    // Get CPU time information about current process.
    if(times(&time_info) == (clock_t) -1){
        // Handle possible error while getting information.
        perror("There was an error getting the system and user times for process.");
        return EXIT_FAILURE;
    }
    
    // Print information.
    printf("The user CPU time is: %f\n", (double) time_info.tms_utime/CLOCKS_PER_SEC);
    printf("The system CPU time is: %f\n", (double) time_info.tms_stime/CLOCKS_PER_SEC);
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
    } else {
        sleep(2);
    }
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
    printf("============================================================================\n");
}
