#ifndef WAVPLAYER_H
#define WAVPLAYER_H

#include <alsa/asoundlib.h>

typedef struct {
	int numSamples;
	short *pData;
} wavedata_t;

snd_pcm_t *Audio_openDevice();
void Audio_readWaveFileIntoMemory(char *fileName, wavedata_t *pWaveStruct);
void Audio_playFile(snd_pcm_t *handle, wavedata_t *pWaveData);
void playTone(void);
#endif