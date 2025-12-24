#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "common.h"

struct Transform {
	Vector2 position;
	Vector2 velocity;
	ushort width = 50;
	ushort height = 50;
	float rotation = 0;
	byte mass = 1;
	char direction = 1;
	char speed = 1;


	operator Rect() {
		Rect r = {
			SDL_lround(position.x),
			SDL_lround(position.y),
			width,
			height,
		};

		return r;


	}

	Transform() {}
};

class MaxObject {
private:


public:
	


	bool mIsActive = true;

	bool mUsePhysics = true;
	bool mIsSolid = true;

	Transform mTransform;

	virtual void Step() {

	}

	void Update() {
		if (!mIsActive) return;
		Step();
	}

	MaxObject() {}

};


#endif