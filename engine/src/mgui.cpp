
#include "mgui.h"
#include "input.h"

Canvas* gCurrentCanvas = nullptr;


bool MouseIsOver(Rect rect) {
	Vector2 m = gInput->GetMousePos();
	return m.x >= rect.x && m.x <= rect.x + rect.w
		&& m.y > +rect.y && m.y <= rect.y + rect.h;
}

Vector2 GetMousePos() {
	return gInput->GetMousePos();
}

void Canvas::UpdateElements() {
	for (ushort i = 0; i < mUsedElements; i++) {
		Element *cur = mElements[i];
		if (MouseIsOver(cur->mRect)) {
			cur->mMouseOver = true;
			if (cur->mIsActive) {
				cur->mRenderColor = STYLE.backgroundHighlighted;
				if (gInput->GetMouseDown(SDL_BUTTON_LEFT)) {
					cur->OnClicked();

				}
			}
		}
		else {
			cur->mRenderColor = STYLE.background;
			cur->mMouseOver = false;
		}
		cur->Update();

	}
}

void Canvas::RenderElements() {
	for (ushort i = 0; i < mUsedElements; i++) {
		Element* cur = mElements[i];
		if (cur->mVisible) cur->Render();
	}
}

Canvas::Canvas() {
	gCurrentCanvas = this;
}
