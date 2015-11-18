#include "main.h"

void get_cookies(int client_id, CLIENT * client){
    srand(time(NULL));
    int response = 0;
    int sleep_time = 0;
    
    while(response != -2){
        sleep_time = rand() % 5;
        sleep(sleep_time);
        response = get_me_my_cookie_1(&client_id, client);
        if(response == -1){
            printf("Aw Tina ask for another cookie so I can have one!\n");
        } else {
            printf("Yay I got a cookie!\n");
        }
    }
    printf("Aw out of cookies I will leave now.\n");
    return;
}