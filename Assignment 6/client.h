#include "common.h"

int run_client(int argc, char * argv[], int client_id){
    srand(time(NULL));
    
    CLIENT * client;
    char * hostname;
    int * response = 0;
    int keep_going = 0;
    char * name;
    
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
        sleep(rand() % 5);
        struct arguments a = {0, client_id};
        response = get_me_my_cookie_1(&a, client);
        
        if(*response == 1){
            print_time();
            printf("CLIENT: Yay I, %s, got a cookie.\n", name);
        }
        if(*response == -1){
            print_time();
            printf("CLIENT: Aw I, %s, have to wait for a cookie.\n", name);
        }
        
        if(*response == -2){
            keep_going = 1;
        }
    }
    print_time();
    printf("CLIENT: Aw I, %s, see that there are no more cookies. I will leave.\n", name);
    return EXIT_SUCCESS;
}