#include "hardwareUpdate.h"
#include "joystick.h"
#include "display.h"
#include "button.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "util.h"
#include "beatGenerator.h"

#define DRUM_SOUND "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_SOUND "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_SOUND "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"

static bool quit = false;
static int mode = 0;
static int BPM = 120;
static int volume = 80;
static int joystickDir = 0;
static pthread_t hardwareUpdateThreadID;
static pthread_mutex_t hwMutex = PTHREAD_MUTEX_INITIALIZER;

//center - 0
//up     - 1
//down   - 2
//left   - 3
//right  - 4
static void updateVolume(int joystickReading){
    if (joystickReading == 1){
        volume = volume + 5;
        if (volume >= 99){
            volume = 99;
        }
    }
    if (joystickReading == 2){
        if (volume <= 5){
            volume = 0;
        }
        else{
            volume = volume - 5;
        }
    }
    // printf("volume ID is: %d\n", volume);
}

static void updateBPM(int joystickReading){
    if (joystickReading == 3){
        BPM = BPM + 5;
        if (BPM >= 300){
            BPM = 300;
        }
    }
    if (joystickReading == 4){
        if (BPM <= 45){
            BPM = 40;
        }
        else{
            BPM = BPM - 5;
        }
    }
    // printf("BPM is: %d\n", BPM);
}

static void updateMode(int grayButtonReading){
    if (grayButtonReading == 1){
        mode++;
        if (mode == 3){
            mode = 0;
        }
        // printf("Mode ID is: %d\n", mode);
    }

}

static void playButtonSound(int buttonReading){

    if (buttonReading != 0 && buttonReading != 1){
        sleep_for_ms(500);
        pthread_mutex_lock(&hwMutex);
       switch (buttonReading)
        {
        case 2:
            printf("playing base drum\n");
            makeSingleTone(2);
            break;
        case 3: 
            printf("playing snare\n");
            makeSingleTone(3);
            // playTone(HIHAT_SOUND);
            break;
        case 4:
            printf("playin hi-hat\n");
            makeSingleTone(4);
            // playTone(SNARE_SOUND);
        default:
            break;
        } 
        sleep_for_ms(500);
        pthread_mutex_unlock(&hwMutex);
    }

}

static void *hardwareUpdate(void *_){
    while (!quit){
        joystickDir = getDirection(joyStickReadX(),joyStickReadY());
        int buttonValue = whichButtonPressed();
        updateVolume(joystickDir);
        updateBPM(joystickDir);
        updateMode(buttonValue);
        playButtonSound(buttonValue);
        sleep_for_ms(300);
    }
    return NULL;
}


void HWupdateInit(void){
    pthread_create(&hardwareUpdateThreadID, NULL, &hardwareUpdate, NULL);
}

void HWupdateStop(void){
    printf("Stopping hardware update thread\n");
    quit = true;
    pthread_join(hardwareUpdateThreadID, NULL);
    printf("Finished stopping hardware updater thread\n\n");
}

int getVolume(){return volume;}
int getBPM(){return BPM;}
int getMode(){return mode;}
int getJoystickDir(){return joystickDir;}