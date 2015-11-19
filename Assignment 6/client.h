#include "common.h"

int run_client(int argc, char * argv[], int client_id){
    srand(time(NULL));
    
    CLIENT * client;
    char * hostname;
    int * response = 0;
    int keep_going = 0;
    char * name;
    int sleep_time = 0;
    struct arguments args = {client_id};
    
    if(client_id == JUDY){
        name = "Judy";
    } else {
        name = "Tina";
    }
    
    if(argc != 2){
        printf("Usage: ./client_name HOSTNAME\n");
        return EXIT_FAILURE;
    }
    
    hostname = argv[1];
    client = clnt_create(hostname, SLW_COOKIES, ASSIGNMENT_6, "udp");
    
    if(client == NULL){
        clnt_pcreateerror("Error creating client.\n");
        exit(EXIT_FAILURE);
    }
    
    while(keep_going == 0){
        sleep_time = rand() % 5;
        printf("[%s] CLIENT: I, %s, will sleep for %d seconds.\n", get_time(), name, sleep_time);
        sleep();
        printf("[%s] CLIENT: I, %s, will ask for a cookie.\n", get_time(), name);
        response = get_me_my_cookie_1(&args, client);
        
        if(*response == 1){
            printf("[%s] CLIENT: Yay I, %s, got a cookie.\n", get_time(), name);
        }
        if(*response == -1){
            printf("[%s] CLIENT: Aw I, %s, have to wait for a cookie.\n", get_time(), name);
        }
        
        if(*response == -2){
            keep_going = 1;
        }
    }
    
    printf("[%s] CLIENT: Aw I, %s, see that there are no more cookies. I will go to bed.\n", get_time(), name);
    bed_time_1(&args, client);
    return EXIT_SUCCESS;
}