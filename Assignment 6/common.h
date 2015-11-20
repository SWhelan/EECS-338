#include <stdlib.h>
#include <stdio.h>
#include "cookie.h"

#define TINA 1
#define JUDY 2

// Returns the current time as a char *
char * get_time(){
    time_t current_time;
    time(&current_time);
    char * time_string;
    time_string = (char *)ctime(&current_time);
    //http://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
    time_string[strcspn(time_string, "\n")] = 0;
    return time_string;
}