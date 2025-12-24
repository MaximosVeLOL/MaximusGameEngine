#ifndef __AUDIO_SYSTEM_H__
#define __AUDIO_SYSTEM_H__

#include <SDL3_mixer/SDL_mixer.h>


struct AudioFile {
	MIX_Audio
};


class AudioSystem {
private:
};

extern AudioSystem* gAudioSystem;

#endif