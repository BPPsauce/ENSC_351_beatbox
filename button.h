#ifndef BUTTON_H
#define BUTTON_H


//This is the function that figure out which button is being pressed
int whichButtonPressed();

//pin config for thr gpio pins
void buttonsInit();
void buttonCleanup();

#endif