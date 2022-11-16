//main.c
#include <stdio.h>
#include "util.h"
#include "button.h"
#include "display.h"
#include "joystick.h"
#include "audioMixer.h"
#include <stdbool.h>
#include <ctype.h>
#include "hardwareUpdate.h"

#include <alsa/asoundlib.h>

#define DRUM_SOUND "beatbox-wav-files/100060__menegass__gui-drum-splash-hard.wav"

int main(int argc, char *argv[])
{
//AUDIO PART
    //  playTone();
    AudioMixer_init();
    wavedata_t pSound;
    AudioMixer_readWaveFileIntoMemory(DRUM_SOUND, &pSound);
    while(1)
    {
        AudioMixer_queueSound(&pSound);
        AudioMixer_queueSound(&pSound);
        sleep_for_ms(4000); //100 is too little for the sound 
        AudioMixer_queueSound(&pSound);
        sleep_for_ms(4000);
    }
//END OF WAV PLAYER
 

    printf("Welcome to the program!\n");
    //pinInit is for the USER button
    buttonsInit();
    updateInit();


    int buttonIspressed = whichButtonPressed();

    while (buttonIspressed != 3){
        //update variables

        //display init
        displayInit();

        buttonIspressed = whichButtonPressed();
        printf("This button is getting prssed : %d\n", buttonIspressed);

    }
    //break loop
    printf("Shutting down...\n");
    updateStop();
    //reset display
    //stop the threads
    resetDisplay();

    printf("Done shutdown! Goodbye!\n");
}