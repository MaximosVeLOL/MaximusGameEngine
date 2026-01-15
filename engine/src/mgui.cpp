
#include "mgui.h"
#include "input.h"

Canvas* gCurrentCanvas = nullptr;


bool MouseIsOver(Rect rect) {
	Vector2 m = gInput->GetMousePos();
	return m.x >= rect.x && m.x <= rect.x + rect.w
		&& m.y > +rect.y && m.y <= rect.y + rect.h;
}

Vector2 GetMousePos() {
	if (!gInput) return Vector2();
	return gInput->GetMousePos();
}

void Canvas::UpdateElements() {
	for (ushort i = 0; i < mUsedElements; i++) {
		if (MouseIsOver(mElements[i]->mRect)) {
			mElements[i]->mMouseOver = true;
			if (mElements[i]->mIsActive) {
				mElements[i]->mRenderColor = STYLE.backgroundHighlighted;
				if (gInput->GetMouseDown(SDL_BUTTON_LEFT)) {
					mElements[i]->OnClicked();

				}
			}
		}
		else {
			mElements[i]->mRenderColor = STYLE.background;
			mElements[i]->mMouseOver = false;
		}
		mElements[i]->Update();

	}
}

void Canvas::RenderElements() {
	for (ushort i = 0; i < mUsedElements; i++) {
		if (mElements[i]->mVisible) mElements[i]->Render();
	}
}

Canvas::Canvas() {
	gCurrentCanvas = this;
}
