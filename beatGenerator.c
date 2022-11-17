#include "beatGenerator.h"
#include "util.h"
#include "audioMixer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "hardwareUpdate.h"

#define DRUM_SOUND "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_SOUND "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_SOUND "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"

static wavedata_t drum;
static wavedata_t hihat;
static wavedata_t snare;
static bool quit = false;

static pthread_mutex_t beatPlayMutex = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t tonePlayMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t beatGenerateThreadID;


//beatdelay in miliseconds 
static int beatDelay(int BPM){
    double delayRes = ((60.0 / (double)BPM ) / 2.0) * 1000;
    return (int)delayRes;
}


static void mode_0_Beat(int BPM, int volume){ //120
    int timeDelayBetween = beatDelay(BPM);
    /*hi-hat base*/
    AudioMixer_setVolume(volume);

    AudioMixer_queueSound(&drum);
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&snare);
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat base*/
    AudioMixer_queueSound(&drum);
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&snare);
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
}

static void mode_1_Beat(int BPM, int volume){ //120
    int timeDelayBetween = beatDelay(BPM);
    /*hi-hat base*/
    AudioMixer_setVolume(volume);

    AudioMixer_queueSound(&drum);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&drum);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&hihat);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&drum);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat base*/
    AudioMixer_queueSound(&drum);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    AudioMixer_queueSound(&snare);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&drum);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&drum);
    sleep_for_ms(timeDelayBetween);
}



static void *beatGenerateThread(void *_){
    while (!quit){
        int mode = getMode();
        int BPM = getBPM();
        int volume = getVolume();
        pthread_mutex_lock(&beatPlayMutex);
        switch (mode)
        {
        case 0:
            mode_0_Beat(BPM, volume);
            break;
        case 1: 
            mode_1_Beat(BPM, volume);
            break;
        default:
            printf("Mode 2: no drum sound\n");
            break;
        }
        pthread_mutex_unlock(&beatPlayMutex);
        sleep_for_ms(200);
    }
    return NULL;
}

void beatPlayerInit(void){
    AudioMixer_readWaveFileIntoMemory(DRUM_SOUND, &drum);
    AudioMixer_readWaveFileIntoMemory(HIHAT_SOUND, &hihat);
    AudioMixer_readWaveFileIntoMemory(SNARE_SOUND, &snare);
    pthread_create(&beatGenerateThreadID, NULL, &beatGenerateThread, NULL);
}

void beatPlayerStop(void){
    quit = true;
    AudioMixer_freeWaveFileData(&drum);
    AudioMixer_freeWaveFileData(&hihat);
    AudioMixer_freeWaveFileData(&snare);
    AudioMixer_cleanup();
    pthread_join(beatGenerateThreadID, NULL);
}