#include "common.h"

int num_cookies = 20;
int tina_count = 0;

int * get_me_my_cookie_1_svc(struct arguments * args, struct svc_req * rqstp){
    
    static int response;
    if(args->client_id == TINA){
        print_time();
        printf("MOTHER: Tina requests a cookie.\n");
    } else {
        print_time();
        printf("MOTHER: Judy requests a cookie.\n");
    }
    fflush(NULL);
    if(num_cookies == 0){
        response = -2;
        print_time();
        printf("MOTHER: No more cookies for you.\n");
    } else if(args->client_id == TINA){
        response = 1;
        num_cookies--;
        tina_count++; 
    } else {
        if(tina_count > 1){
            response = 1;
            num_cookies--;
            tina_count = 0;
        } else {
            print_time();
            printf("MOTHER: Tina hasn't gotten two cookies yet you'll have to wait.\n");
            response = -1;
        }
    }
    return(&response);
    
}