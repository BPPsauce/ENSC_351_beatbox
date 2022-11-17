#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//#define USER_BUTTON "/sys/class/gpio/gpio72/value"
#define GRAY_MODE "/sys/class/gpio/gpio47/value"
#define RED_BASE "/sys/class/gpio/gpio46/value"
#define YELLOW_SNARE "/sys/class/gpio/gpio27/value"
#define GREEN_HIHAT "/sys/class/gpio/gpio65/value"



/*determine if this certain button is pressed*/
static int isPressedButton(char* GPIO){
    int result = file_read(GPIO);
    if (result == 1){//when pressed, its 0
        return 1;
    }
    return 0;
}


/*determine which button is being prsses and return the button number*/
int whichButtonPressed(){
    if (isPressedButton(GRAY_MODE) == 1){
        return 1;
    }
    else if (isPressedButton(RED_BASE) == 1){
        return 2;
    }
    else if (isPressedButton(YELLOW_SNARE) == 1){
        return 3;
    }
    else if (isPressedButton(GREEN_HIHAT) == 1){
        return 4;
    }
    return 0;
}


/*pin config for the GPIO pin for BBG*/
void buttonsInit(){
    //setting the buttons to gpio
    //runCommand("config-pin P8.43 gpio");
    runCommand("config-pin P8.15 gpio");
    runCommand("config-pin P8.16 gpio");
    runCommand("config-pin P8.17 gpio");
    runCommand("config-pin P8.18 gpio");
}

/*for the sake of looking nice and organized*/
void buttonCleanup(){
    printf("Buttons clean up\n");
}