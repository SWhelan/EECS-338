#include "common.h"

int num_cookies = 20;
int tina_count = 0;

int * get_me_my_cookie_1_svc(struct arguments * args, struct svc_req * rqstp){
    
    static int response;
    if(args->client_id == TINA){
        printf("[%s] MOTHER: I received Tina's request for a cookie.\n", get_time());
    } else {
        printf("[%s] MOTHER: I received Judy's request for a cookie.\n", get_time());
    }
    fflush(NULL);
    if(num_cookies == 0){
        response = -2;
        printf("[%s] MOTHER: No more cookies for you.\n", get_time());
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
            printf("[%s] MOTHER: Tina hasn't gotten two cookies yet you'll have to wait.\n", get_time());
            response = -1;
        }
    }
    return(&response);
    
}