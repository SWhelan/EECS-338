#include <stdlib.h>
#include <stdio.h>
#include "cookie.h"

#define TINA 1
#define JUDY 2

void print_time(){
    time_t current_time;
    time(&current_time);
    printf("The current time is: %s", ctime(&current_time));
}