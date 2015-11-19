#include "common.h"

int num_cookies = 20;
int tina_count = 0;
int tina_asleep = 0;
int judy_asleep = 0;

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
        printf("[%s] MOTHER: I gave Tina a cookie.\n", get_time());
    } else {
        if(tina_count > 1){
            response = 1;
            num_cookies--;
            tina_count = 0;
            printf("[%s] MOTHER: I gave Judy a cookie.\n", get_time());
        } else {
            printf("[%s] MOTHER: Tina hasn't had two cookies yet you have to wait.\n", get_time());
            response = -1;
        }
    }
    return(&response);
    
}

void * bed_time_1_svc(struct arguments * args, struct svc_req * rgstp){
    if(args->client_id == TINA){
        tina_asleep = 1;
        printf("[%s] MOTHER: Tina went to bed.\n", get_time());
    }
    
    if(args->client_id == JUDY){
        judy_asleep = 1;
        printf("[%s] MOTHER: Judy went to bed.\n", get_time());
    }
    
    if(tina_asleep == 1 && judy_asleep == 1){
        printf("[%s] MOTHER: Everyone went to bed now I can have a cookie.\n", get_time());
        exit(EXIT_SUCCESS);
    }
    
}
	