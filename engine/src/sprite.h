#pragma once

#include "common.h"
#include "ezrender.h"

struct Sprite {
	SDL_Texture* page = nullptr;
	Rect renderRect;
};