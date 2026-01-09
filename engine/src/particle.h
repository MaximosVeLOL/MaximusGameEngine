#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "common.h"
#include "sprite.h"
class Particle {
public:
	Vector2 mPosition;
	float mRotation = 0;
	Sprite renderSprite;
};

#endif