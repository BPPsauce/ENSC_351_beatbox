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
    HWupdateInit();
    Interval_init();
    AudioMixer_init();
    beatPlayerInit();
    console_writer_init();

    printf("Enter Q to quit.\n");
    while(true)
    {
        if(toupper(getchar() == 'Q'))
        {
            break;
        }
    }
    //break loop
    printf("Shutting down...\n");
    console_writer_cleanup();
    beatPlayerStop();
    AudioMixer_cleanup();
    HWupdateStop();
    
    //reset display
    //stop the threads
    // resetDisplay();

    printf("Done shutdown! Goodbye!\n");
    }
