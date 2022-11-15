//this file is the separate thread for updating all the variables according to the input from the hardware modules
#ifndef HARDWAREUPDATE_H
#define HARDWAREUPDATE_H

void updateInit(void);

void updateStop(void);

/*getters for the variables*/
int getVolume();
int getBPM();
int getMode();

#endif