#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define USER_BUTTON "/sys/class/gpio/gpio72/value"

int isPressedButton(){
    int result = file_read(USER_BUTTON);
    if (result == 0){//when pressed, its 0
        return 1;
    }
    return 0;
}

void pinInit(){
    runCommand("config-pin P8.43 gpio");
}