#include "ezrender.h"
#include "instance.h"

EzRender* gEzRender = nullptr;


EzRender::EzRender() {
	SDL_Log("Initializing renderer...");
#if COMOPT_C_CLIENT
#if COMOPT_R_USE_HA 
	mRenderer = SDL_CreateRenderer(gInstance->mWindow, NULL);
	SDL_SetRenderLogicalPresentation(mRenderer, RENDER_WIDTH, RENDER_HEIGHT, SDL_LOGICAL_PRESENTATION_DISABLED);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer!");
	}
#endif
#endif
	gEzRender = this;
	SDL_Log("Created renderer (Success!)");
}