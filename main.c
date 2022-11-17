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

/*welcome message*/
    printf("Welcome to the program!\n");
  
    /*init all the threads*/
    Interval_init();
    beatPlayerInit();
    console_writer_init();
    buttonsInit();
    HWupdateInit();
    displayWriter_init();
    AudioMixer_init();

    printf("Enter Q to quit.\n");
    while(true)
    {
        if(toupper(getchar() == 'Q'))
        {
            break;
        }
    }
    //break loop
    /*clean up*/
    AudioMixer_cleanup();
    printf("Shutting down...\n");
    displayWriter_cleanup();
    HWupdateStop();
    buttonCleanup();
    console_writer_cleanup();
    beatPlayerStop();
    Interval_cleanup();

    printf("Done shutdown! Goodbye!\n");
    }
