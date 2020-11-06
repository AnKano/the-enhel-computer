/************************************************************************/
/*			KEGS: Apple //gs Emulator			*/
/*			Copyright 2002 by Kent Dickey			*/
/*									*/
/*		This code is covered by the GNU GPL			*/
/*									*/
/*	The KEGS web page is kegs.sourceforge.net			*/
/*	You may contact the author at: kadickey@alumni.princeton.edu	*/
/************************************************************************/

#include "defc.h"
#include "sound.h"
#include <unistd.h>

extern int Verbose;

extern int g_audio_rate;

void check_wave_error(int res, char *str);

extern int g_audio_enable;
extern word32 *g_sound_shm_addr;
extern int g_preferred_rate;

void win32snd_init(word32 *shmaddr) {
    child_sound_loop(-1, -1, shmaddr);
}

void win32snd_shutdown() {}

void check_wave_error(int res, char *str) {}

void child_sound_init_win32() {
    set_audio_rate(g_audio_rate);
}

extern unsigned int *audio_buffer_count;
extern byte **audio_buffer;

struct SoundEvent {
    byte *buffer;
    unsigned int size;
    struct SoundEvent *_nextVal;
};

extern struct SoundEvent *soundLastSample;

int win32_send_audio(byte *ptr, int in_size) {
    *audio_buffer = ptr;
    *audio_buffer_count = in_size;

    // It's huge because the simplest one way create audio error interrupts
    if (soundLastSample == NULL) {
        soundLastSample = malloc(sizeof(struct SoundEvent));
        soundLastSample->size = in_size;
        soundLastSample->buffer = malloc(sizeof(byte) * soundLastSample->size);
        soundLastSample->_nextVal = NULL;

        memcpy(soundLastSample->buffer, ptr, soundLastSample->size);
    } else {
        soundLastSample->_nextVal = malloc(sizeof(struct SoundEvent));
        soundLastSample = soundLastSample->_nextVal;

        soundLastSample->size = in_size;
        soundLastSample->buffer = malloc(sizeof(byte) * soundLastSample->size);
        soundLastSample->_nextVal = NULL;

        memcpy(soundLastSample->buffer, ptr, soundLastSample->size);
    }

    return in_size;
}
