#pragma once

class MaxObject;
#include "common.h"

typedef byte olimit;
constexpr olimit OBJECT_LIMIT = 100;

class World {
	MaxObject* mObjects[OBJECT_LIMIT] = { nullptr };
	olimit mObjectCount = 0;

	byte mGravity = 1;
	Vector2 mCameraPosition;

public:

	void AddObject(MaxObject* object) {
		mObjects[mObjectCount] = object;
		mObjectCount++;
		if (mObjectCount > OBJECT_LIMIT) {
		}
	}


	void UpdateAllObjects();

	void RenderAllObjects();

	World();

};

struct StaticMaxObject {
	const char* ID = "";
	Vector2 position;
};

struct WorldSpace {
};

extern World* gCurrentWorld;