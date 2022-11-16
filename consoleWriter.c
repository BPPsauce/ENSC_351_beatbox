//consoleWriter.c
#include "consoleWriter.h"
#include "intervalTimer.h"
#include "beatGenerator.h"
#include "audioMixer.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t writerThread;
static bool stopThread;

static void *console_writer_thread(void *_)
{
    Interval_statistics_t stats;
    while(!stopThread)
    {
        int volume = AudioMixer_getVolume();
        Interval_getStatisticsAndClear(INTERVAL_LOW_LEVEL_AUDIO, &stats);
        printf("vol: %d \tLow [%f, %f] avg %f/%d\n", volume, stats.minIntervalInMs, stats.maxIntervalInMs, stats.avgIntervalInMs, stats.numSamples);
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
    stopThread = true;
    pthread_join(writerThread, NULL);
}