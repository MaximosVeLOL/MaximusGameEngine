#include "audiosystem.h"


AudioSystem* gAudio = nullptr;

AudioSystem::AudioSystem() {
	if (!MIX_Init()) {
		SDL_Log("Failed to initialize SDL_mixer!");
		return;
	}
	mMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!mMixer) {
		SDL_Log("Failed to create mixer!");
		return;
	}
	for (byte i = 0; i < 8; i++) {
		mTracks[i] = MIX_CreateTrack(mMixer);
	}

	gAudio = this;
}
