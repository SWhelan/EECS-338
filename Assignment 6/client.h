#include "common.h"

/*
 * Shared client method for both tina and judy.
 * Both tina and judy start and call this method.
 * While there are still cookies to be had the client loops
 * randomly sleeps for a small amount of time then asks for a cookie
 * via an RPC method.
 * Prints out a message at each step.
 * Once there aren't any more cookies the method returns.
 */
int run_client(int argc, char * argv[], int client_id){
    // Seed the random number generator
    srand(time(NULL));
    
    // Client is the RPC client
    CLIENT * client;
    
    // Hostname to be retrieved from command line arguments
    char * hostname;
    
    // The response from the server
    int * response = 0;
    
    // Flag to determine if there are still cookies
    int cookies_exist = 1;
    
    // The name of the client to print
    char * name;
    
    // Variable to hold the amount of time to sleep/print how long slept
    int sleep_time = 0;
    
    // The arguments to pass to the RPC calls
    struct arguments args = {client_id};
    
    // Set the name of the client to print
    if(client_id == JUDY){
        name = "Judy";
    } else {
        name = "Tina";
    }
    
    // Get the host name from the command line arguments
    if(argc != 2){
        printf("Usage: ./client_name HOSTNAME\n");
        return EXIT_FAILURE;
    }
    
    hostname = argv[1];
    
    // Create the RPC client (see cookie.x)
    client = clnt_create(hostname, SLW_COOKIES, ASSIGNMENT_6, "udp");
    
    // Check if there was an error creating client
    if(client == NULL){
        clnt_pcreateerror("Error creating client.\n");
        exit(EXIT_FAILURE);
    }
    
    // While there are still cookies keep asking for cookies
    while(cookies_exist == 1){
        
        // Sleep a random amount of time (0 to 4 seconds)
        sleep_time = rand() % 5;
        printf("[%s] CLIENT: I, %s, will sleep for %d seconds.\n", get_time(), name, sleep_time);
        sleep(sleep_time);
        
        // Ask for a cookie
        printf("[%s] CLIENT: I, %s, will ask for a cookie.\n", get_time(), name);
        response = get_me_my_cookie_1(&args, client);
        
        // Determine if a cookie was received
        if(*response == 1){
            printf("[%s] CLIENT: Yay I, %s, got a cookie.\n", get_time(), name);
        }
        if(*response == -1){
            printf("[%s] CLIENT: Aw I, %s, have to wait for a cookie.\n", get_time(), name);
        }
        
        // Out of cookies
        if(*response == -2){
            cookies_exist = 0;
        }
    }
    
    // Exit when out of cookies
    printf("[%s] CLIENT: Aw I, %s, see that there are no more cookies. I will go to bed.\n", get_time(), name);
    
    // Also tell the server when exiting so the server can terminate after all clients have left
    bed_time_1(&args, client);
    
    // Destroy the RPC client
    clnt_destroy(client);
    
    // Exit
    return EXIT_SUCCESS;
}