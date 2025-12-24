#include "world.h"
#include "object.h"
#include "ezrender.h"
#include "instance.h"

World* gCurrentWorld = nullptr;


World::World() {
	gCurrentWorld = this;
}

bool IsColliding(bool useXAxis, Vector2 firstPoint, ushort firstWidth, ushort firstHeight, Vector2 secondPoint, ushort secondWidth, ushort secondHeight) {
	if (useXAxis) {
		return	
			firstPoint.x >= secondPoint.x &&
			firstPoint.x <= secondPoint.x + secondWidth &&
			firstPoint.y > secondPoint.y &&
			firstPoint.y < secondPoint.y + secondHeight
			||
			firstPoint.x >= secondPoint.x &&
			firstPoint.x <= secondPoint.x + secondWidth &&
			firstPoint.y + firstHeight > secondPoint.y &&
			firstPoint.y + firstHeight < secondPoint.y + secondHeight;
	}
	else {
		return	
			firstPoint.x >= secondPoint.x &&
			firstPoint.x <= secondPoint.x + secondWidth &&
			firstPoint.y > secondPoint.y &&
			firstPoint.y < secondPoint.y + secondHeight
			||
			firstPoint.x + firstWidth >= secondPoint.x &&
			firstPoint.x + firstWidth <= secondPoint.x + secondWidth &&
			firstPoint.y > secondPoint.y &&
			firstPoint.y < secondPoint.y + secondHeight;
	}
}

void World::UpdateAllObjects() {
	for (olimit i = 0; i < mObjectCount; i++) {
		MaxObject& cur = *mObjects[i];
		cur.Update();
		if (cur.mUsePhysics) cur.mTransform.velocity.y += cur.mTransform.mass * mGravity;
		if (!cur.mIsSolid) continue;

		Vector2 top;
		for (olimit collisionI = 0; collisionI < mObjectCount; collisionI++) {
			if (collisionI == i) continue;
			MaxObject& other = *mObjects[collisionI];
			if (!other.mIsSolid || !other.mIsActive) continue;

			//Left collision
			top = Vector2(cur.mTransform.position.x + cur.mTransform.velocity.x, cur.mTransform.position.y);
			constexpr byte movePos = 1;
			if (IsColliding(true, top, 0, cur.mTransform.height, other.mTransform.position, other.mTransform.width, other.mTransform.height)) {
				cur.mTransform.position.x = other.mTransform.position.x + other.mTransform.width + 1;
				cur.mTransform.velocity.x = 0;
			}
			//Right collision
			top = Vector2(cur.mTransform.position.x + cur.mTransform.width + cur.mTransform.velocity.x, cur.mTransform.position.y);
			if (IsColliding(true, top, 0, cur.mTransform.height, other.mTransform.position, other.mTransform.width, other.mTransform.height)) {
				cur.mTransform.position.x = other.mTransform.position.x - cur.mTransform.width - 1;
				cur.mTransform.velocity.x = 0;
			}

			//Top collision
			top = Vector2(cur.mTransform.position.x + cur.mTransform.velocity.x, cur.mTransform.position.y + cur.mTransform.velocity.y);
			if (IsColliding(false, top, cur.mTransform.width, 0, other.mTransform.position, other.mTransform.width, other.mTransform.height)) {
				cur.mTransform.position.y = other.mTransform.position.y + other.mTransform.height;
				cur.mTransform.velocity.y = 0;
			}
			//Bottom collision
			top = Vector2(cur.mTransform.position.x + cur.mTransform.velocity.x, cur.mTransform.position.y + cur.mTransform.velocity.y + cur.mTransform.height);
			if (IsColliding(false, top, cur.mTransform.width, 0, other.mTransform.position, other.mTransform.width, other.mTransform.height)) {
				cur.mTransform.position.y = other.mTransform.position.y - cur.mTransform.height;
				cur.mTransform.velocity.y = 0;
			}
			
		}

		cur.mTransform.position.x += cur.mTransform.velocity.x;
		cur.mTransform.position.y += cur.mTransform.velocity.y;
	}
}

void World::RenderAllObjects() {
	if (!gEzRender)
		return;
	gEzRender->cSetDrawColor(0, 255, 0, 255);
	for (olimit i = 0; i < mObjectCount; i++) {
		gEzRender->cRenderRect(mObjects[i]->mTransform, true);
	}
}