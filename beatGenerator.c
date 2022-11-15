#include "beatGenerator.h"
#include "util.h"


#define DRUM_SOUND "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_SOUND "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_SOUND "beatbox-wav-files/100059__menegass__gui-drum-snare-soft"


void beatGenerate(int mode);

void playBeat();

//beatdelay in miliseconds 
int beatDelay(int BPM){
    int delayRes = (60 / BPM / 2) * 1000;
    return delayRes;
}