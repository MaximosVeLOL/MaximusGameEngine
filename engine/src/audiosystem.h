#ifndef __AUDIO_SYSTEM_H__
#define __AUDIO_SYSTEM_H__

#include "common.h"
#include <SDL3_mixer/SDL_mixer.h>


struct Sound {
	MIX_Audio* mAudioData = nullptr;
	bool mIsPlaying = false;

	Sound(void* pData) {
		if (!pData) return;
		mAudioData = reinterpret_cast<MIX_Audio*>(pData);
	}
};


class AudioSystem {
private:
	
	MIX_Track* mTracks[8] = {nullptr};
public:
	MIX_Mixer* mMixer = nullptr;
	AudioSystem();

	void PlaySound(Sound pSound, char pWhichChannel = -1) {
		if (pWhichChannel < 0) {
			pWhichChannel = 0; //Temp code
		}
		MIX_SetTrackAudio(mTracks[pWhichChannel], pSound.mAudioData);
		MIX_PlayTrack(mTracks[0], 1);
	}
};

extern AudioSystem* gAudio;

#endif