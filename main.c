//main.c
#include <stdio.h>
#include "util.h"
#include "button.h"
#include "display.h"
#include "joystick.h"
#include "audioMixer.h"
#include <stdbool.h>
#include <ctype.h>

#include <alsa/asoundlib.h>

#define DRUM_SOUND "beatbox-wave-files/100060__menegass__gui-drum-splash-hard.wav"

int main(int argc, char *argv[])
{
//AUDIO PART
    //  playTone();
    while(1)
    {  
        AudioMixer_init();
        wavedata_t pSound;
        AudioMixer_readWaveFileIntoMemory(DRUM_SOUND, &pSound);
        AudioMixer_queueSound(&pSound);
        sleep_for_ms(10);
    }
//END OF WAV PLAYER
 
    //variable init
    int mode = 0;

    printf("Welcome to the program!\n");
    //pinInit is for the USER button
    buttonsInit();

    int buttonIspressed = whichButtonPressed();
    printf("This button is getting prssed : %d\n", buttonIspressed);

    while (buttonIspressed != 3){
        //update variables

        //display init
        displayInit();

        int direction = getDirection(joyStickReadX(), joyStickReadY());
            //center - 0
            //up     - 1
            //down   - 2
            //left   - 3
            //right  - 4
        switch (direction)
        {
        case 0: //center
            printMode(mode);
            break;
        case 1: //up
            printf("Pressing Up\n");
            //print volume
            break;
        case 2: //down 
            printf("Pressing Down\n");
            //print volume
            break;
        case 3: //left
            printf("Pressing Left\n");
            //print BPM
            break;
        case 4: //right
            printf("Pressing Right\n");
            //print BPM 
            break;
        default:
            printf("Joystick reading not correct!\n");
            break;
        }
        buttonIspressed = whichButtonPressed();
        printf("This button is getting prssed : %d\n", buttonIspressed);

    }
    //break loop
    printf("Shutting down...\n");
    //reset display
    //stop the threads
    resetDisplay();

    printf("Done shutdown! Goodbye!\n");
}