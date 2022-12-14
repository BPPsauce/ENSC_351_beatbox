#include "beatGenerator.h"
#include "util.h"
#include "audioMixer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "hardwareUpdate.h"
#include "intervalTimer.h"

/*sound define*/
#define DRUM_SOUND "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_SOUND "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_SOUND "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
static wavedata_t drum;
static wavedata_t hihat;
static wavedata_t snare;

/*thread quit flag*/
static bool quit = false;
/*thread ID*/
static pthread_t beatGenerateThreadID;


//beatdelay in miliseconds 
static int beatDelay(int BPM){
    double delayRes = ((60.0 / (double)BPM ) / 2.0) * 1000;
    return (int)delayRes;
}

/*standard rock beat*/
static void mode_0_Beat(int BPM, int volume){ //120
    int timeDelayBetween = beatDelay(BPM);
    /*hi-hat base*/
    AudioMixer_setVolume(volume);

    AudioMixer_queueSound(&drum);
    AudioMixer_queueSound(&hihat);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&snare);
    AudioMixer_queueSound(&hihat);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
}

/*custome beat*/
static void mode_1_Beat(int BPM, int volume){ //120
    int timeDelayBetween = beatDelay(BPM);
    /*hi-hat base*/
    AudioMixer_setVolume(volume);

    AudioMixer_queueSound(&drum);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&drum);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&hihat);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&drum);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat base*/
    AudioMixer_queueSound(&drum);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&hihat);
    AudioMixer_queueSound(&snare);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat snare*/
    AudioMixer_queueSound(&drum);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
    /*hi-hat*/
    AudioMixer_queueSound(&drum);
    Interval_markInterval(INTERVAL_BEAT_BOX);
    sleep_for_ms(timeDelayBetween);
}

/*only makes a single tone*/
void makeSingleTone(int buttonInput){
    int BPM = getBPM();
    int volume = getVolume();
    AudioMixer_setVolume(volume);
    switch (buttonInput)
    {
    case 2:
        AudioMixer_queueSound(&drum);
        sleep_for_ms(beatDelay(BPM));
        break;
    case 3: 
        AudioMixer_queueSound(&snare);
        sleep_for_ms(beatDelay(BPM));
        break;
    case 4:
        AudioMixer_queueSound(&hihat);
        sleep_for_ms(beatDelay(BPM));
        break;
    default:
        break;
    }
}

/*cycling the beat according to the button input*/
static void *beatGenerateThread(void *_){
    while (!quit){
        int mode = getMode();
        int BPM = getBPM();
        int volume = getVolume();
        switch (mode)
        {
        case 0:
            mode_0_Beat(BPM, volume);
            break;
        case 1: 
            mode_1_Beat(BPM, volume);
            break;
        default:
            //printf("Mode 2: no drum sound\n");
            break;
        }
    }
    return NULL;
}

/*thread init*/
void beatPlayerInit(void){
    AudioMixer_readWaveFileIntoMemory(DRUM_SOUND, &drum);
    AudioMixer_readWaveFileIntoMemory(HIHAT_SOUND, &hihat);
    AudioMixer_readWaveFileIntoMemory(SNARE_SOUND, &snare);
    pthread_create(&beatGenerateThreadID, NULL, &beatGenerateThread, NULL);
}

/*thread clean up*/
void beatPlayerStop(void){
    printf("Stopping beat player thread\n");
    quit = true;
    pthread_join(beatGenerateThreadID, NULL);
    AudioMixer_freeWaveFileData(&drum);
    AudioMixer_freeWaveFileData(&hihat);
    AudioMixer_freeWaveFileData(&snare);
    printf("finished stopping beat player thread\n\n");
}