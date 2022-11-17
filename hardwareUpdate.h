//this file is the separate thread for updating all the variables according to the input from the hardware modules
#ifndef HARDWAREUPDATE_H
#define HARDWAREUPDATE_H

/*init for hardware update thread*/
void HWupdateInit(void);
/*clean up*/
void HWupdateStop(void);

/*getters for the variables*/
int getVolume();
int getBPM();
int getMode();
int getJoystickDir();

#endif