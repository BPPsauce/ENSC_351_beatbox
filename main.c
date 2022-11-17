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
#include "beatGenerator.h"
#include "consoleWriter.h"
#include "intervalTimer.h"

#include <alsa/asoundlib.h>


int main(int argc, char *argv[])
{
//AUDIO PART
    //  playTone();
    /*
    AudioMixer_init();
    wavedata_t DrumSound;
    wavedata_t DrumSound2;
    AudioMixer_readWaveFileIntoMemory(DRUM_SOUND, &DrumSound);
    AudioMixer_readWaveFileIntoMemory(DRUM_SOUND_2, &DrumSound2);
    while(1)
    {
        AudioMixer_queueSound(&DrumSound);
        AudioMixer_queueSound(&DrumSound2);
        sleep_for_ms(4000); //100 is too little for the sound 
        AudioMixer_queueSound(&DrumSound2);*/
   
    /*
    wavedata_t pSound;
    AudioMixer_readWaveFileIntoMemory(DRUM_SOUND, &pSound);
    while(1)
    {
        AudioMixer_queueSound(&pSound);
        AudioMixer_queueSound(&pSound);
        sleep_for_ms(4000);
        AudioMixer_queueSound(&pSound);
        sleep_for_ms(4000);
    }*/
//END OF WAV PLAYER
 

    printf("Welcome to the program!\n");
    //pinInit is for the USER button
    buttonsInit();
    updateInit();
    Interval_init();
    AudioMixer_init();
    //Interval_init();
    console_writer_init();
    beatPlayerInit();
    //buttonsInit();
    //updateInit();
    
    int buttonIspressed = whichButtonPressed();

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
            printMode(getMode());
            break;
        case 1: //up
            printInteger(getVolume());
            sleep_for_ms(100);
            break;
        case 2: //down 
            printInteger(getVolume());
            sleep_for_ms(100);
            break;
        case 3: //left
            printDouble(getBPM() / 60);
            sleep_for_ms(100);
            break;
        case 4: //right
            printDouble(getBPM() / 60);
            sleep_for_ms(100);
            break;
        default:
            printf("Joystick reading not correct!\n");
            break;

        buttonIspressed = whichButtonPressed();
        printf("This button is getting prssed : %d\n", buttonIspressed);

        }
    }
    //break loop
    printf("Shutting down...\n");
    updateStop();
    beatPlayerStop();
    //reset display
    //stop the threads
    resetDisplay();

    printf("Done shutdown! Goodbye!\n");
    }
