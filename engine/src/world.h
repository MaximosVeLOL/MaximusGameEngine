#pragma once

class MaxObject;
#include "common.h"
#include <vector> //For uGetObjectsInView

typedef uint olimit;
constexpr olimit OBJECT_LIMIT = 10000;
typedef byte climit;
constexpr climit CAMERA_LIMIT = 10;

struct Camera {
	Vector2 mViewOffset;
	Vector2 mPosition;
	bool mIsActive = true;
	ushort mWidth = 960;
	ushort mHeight = 540;
};


class World {

	friend class WorldSpace;

	MaxObject* mObjects[OBJECT_LIMIT] = { nullptr };
	olimit mObjectCount = 0;

	float mGravity = 0.25;

	ushort mTickRate = 0;
	ushort mTicks = 0;

	uint mWidth = 0;
	uint mHeight = 0;

	Camera mCameras[CAMERA_LIMIT] = {Vector2()};
	climit mCameraCount = 0;
	
public:
	

	Color mBGColor = {
		0,
		255,
		0,
		255
	};



	void AddObject(MaxObject* object);

	MaxObject* uGetObjectInPoint(Vector2 pPoint);
	std::vector<MaxObject *> uGetObjectsInView();

	void UpdateAllObjects();

	void RenderAllObjects();

	MaxObject* Raycast(Vector2 startingPos, Vector2 direction, ushort maxDistance = 100);

	World();

};

extern World* gCurrentWorld;