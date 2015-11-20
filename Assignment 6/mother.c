#include "common.h"

// The initial number of cookies
int num_cookies = 20;

// A count to determine if tina has had at least 2 cookies
int tina_count = 0;

// 0 if tina is running 1 if terminated
int tina_asleep = 0;

// 0 if judy is running 1 if terminated
int judy_asleep = 0;

/*
 * RPC Method (rpcgen cookie.x) that takes the client id (int) in a struct 
 * and hands out cookies.
 * There are initially 20 cookies.
 * Tina may always have a cookie.
 * Judy may only have a cookie if Tina has had at least two cookies since
 * the last time Judy had a cookie.
 *
 * Returns -2 if there are no more cookies
 * Returns -1 if Judy asked but Tina hasn't had two yet
 * Returns +1 if Judy or Tina (whoever asked) may have a cookie 
 */
int * get_me_my_cookie_1_svc(struct arguments * args, struct svc_req * rqstp){
    
    // Store the response statically
    static int response;
    
    // Print that a request for a cookie was received
    if(args->client_id == TINA){
        printf("[%s] MOTHER: I received Tina's request for a cookie.\n", get_time());
    } else {
        printf("[%s] MOTHER: I received Judy's request for a cookie.\n", get_time());
    }
    fflush(NULL);
    
    // Return -2 if there aren't any more cookies
    if(num_cookies == 0){
        response = -2;
        // Print a subtle reference
        printf("[%s] MOTHER: No more cookies for you.\n", get_time());
    } else if(args->client_id == TINA){
        // If there are enough cookies and Tina is asking for one return 1
        response = 1;
        num_cookies--;
        tina_count++; 
        printf("[%s] MOTHER: I gave Tina a cookie.\n", get_time());
    } else {
        // If there are enough cookies and Judy is asking
        if(tina_count > 1){
            // If Tina has had at least two give Judy a cookie
            response = 1;
            num_cookies--;
            tina_count = 0;
            printf("[%s] MOTHER: I gave Judy a cookie.\n", get_time());
        } else {
            // Otherwise don't give Judy a cookie
            printf("[%s] MOTHER: Tina hasn't had two cookies yet you have to wait.\n", get_time());
            response = -1;
        }
    }
    
    // Return the response set by the above logic
    // Has to be a pointer because this is an RPC method
    return(&response);
    
}

/*
 * A RPC method (rpcgen cookie.x) to allow termination of this, 
 * the server process, after both clients have terminated.
 */
void * bed_time_1_svc(struct arguments * args, struct svc_req * rgstp){
    
    // Tina has indicated she is going to bed
    if(args->client_id == TINA){
        tina_asleep = 1;
        printf("[%s] MOTHER: Tina went to bed.\n", get_time());
    }
    
    // Judy has indicated she is going to bed
    if(args->client_id == JUDY){
        judy_asleep = 1;
        printf("[%s] MOTHER: Judy went to bed.\n", get_time());
    }
    
    // Exit the server process
    if(tina_asleep == 1 && judy_asleep == 1){
        printf("[%s] MOTHER: Everyone went to bed now I can have a cookie.\n", get_time());
        exit(EXIT_SUCCESS);
    }
    
}
	