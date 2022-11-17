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


    printf("Welcome to the program!\n");
    buttonsInit();
    displayWriter_init();
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
    displayWriter_cleanup();

    printf("Done shutdown! Goodbye!\n");
    }
