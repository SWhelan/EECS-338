#include "main.h"
#include <rpc/rpc.h>
int num_cookies = 20;
int tina_count = 0;

int get_me_my_cookie_1_svc(int * client_id, struct svc_req * req){
    printf("Hello child I will check the cookie jar.\n");
    
    if(num_cookies == 0){
        printf("No more cookies for you.\n");
        return -2;
    }
    
    if(&client_id == TINA){
        tina_count++;
        num_cookies--;
        return 1;
    }
    
    if(&client_id == JUDY){
        if(tina_count > 1){
            tina_count = 0;
            num_cookies--;
            return 1;
        } else {
            return -1;
        }
    }
}