//display.h
//functions for 8x8 LED matrix
#ifndef DISPLAY_H
#define DISPLAY_H

//print out integer
void printInteger(int reading);

void printDouble(double reading);

void printMode(int mode);

void displayInit(void);

void resetDisplay(void);


#endif