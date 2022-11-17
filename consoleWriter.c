//consoleWriter.c
#include "consoleWriter.h"
#include "intervalTimer.h"
#include "beatGenerator.h"
#include "hardwareUpdate.h"
#include "audioMixer.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_t writerThread;
static bool stopThread;

static void *console_writer_thread(void *_)
{
    Interval_statistics_t stats;
    while(!stopThread)
    {
        int volume = AudioMixer_getVolume();
        int bpm = getBPM();
        int mode = getMode();
        //get statistics for low level
        Interval_getStatisticsAndClear(INTERVAL_LOW_LEVEL_AUDIO, &stats);
        float low_min = stats.minIntervalInMs;
        float low_max = stats.maxIntervalInMs;
        float low_av = stats.avgIntervalInMs;
        int low_num_samples = stats.numSamples;
        
        //get statistics for high level
        Interval_getStatisticsAndClear(INTERVAL_BEAT_BOX, &stats);
        float beat_min = stats.minIntervalInMs;
        float beat_max = stats.maxIntervalInMs;
        float beat_av = stats.avgIntervalInMs;
        int beat_num_samples = stats.numSamples;

        printf("M%d %dbpm vol: %d \tLow [%f, %f] avg %f/%d\t Beat[%f, %f] avg %f/%d\n", 
                mode, bpm, volume, low_min, low_max, low_av, low_num_samples, beat_min, 
                beat_max, beat_av, beat_num_samples);
        sleep_for_ms(1000);
    }
    return NULL;
}

void console_writer_init()
{
    stopThread = false;
    pthread_create(&writerThread, NULL, console_writer_thread, NULL);
}

void console_writer_cleanup(void)
{
    printf("Stopping console writer thread\n");
    stopThread = true;
    pthread_join(writerThread, NULL);
    printf("Finished stopping console writer thread\n\n");
}