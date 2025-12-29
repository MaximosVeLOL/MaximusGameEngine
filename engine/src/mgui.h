
#ifndef __MGUI_H__
#define __MGUI_H__

#include "common.h"
#include "ezrender.h"

constexpr ushort ELEMENT_LIMIT = 100;



// Two colors that have an active and a deactive color
struct GColor {
	Color mActive = {255,255,255,255};
	Color mDeactive = {0,0,0,255};
};

struct GUIStyle {
	Color background = { 144, 0, 0, 255 };
	Color backgroundHighlighted = { 255, 0, 0, 255 };
	Color text = { 0, 0, 0, 255 };
};

const GUIStyle STYLE;

extern bool MouseIsOver(Rect rect);

extern Vector2 GetMousePos();


class Element {
public:
	string_static ID = nullptr;
	Rect mRect = {
		0,
		0,
		50,
		100
	};
	Color mRenderColor;
	bool mIsActive = true; //If we are not active, then we show that.
	bool mVisible = true;
	bool mMouseOver = false;

	void uToggleActive() {
		mIsActive = !mIsActive;
	}

	void uToggleVisibility() {
		mVisible = !mVisible;
	}

	void uRenderRect() {
		gEzRender->cRenderRect(mRect, true, &mRenderColor);
	}

	void uRenderText(string_static format, ...) {
		gEzRender->cSetDrawColor(STYLE.text);
		gEzRender->cRenderText(mRect.x, mRect.y, format);
	}


	virtual void OnClicked() {
		SDL_Log("Clicked!");
	}

	virtual void Step() {
		
	}

	virtual void Render() {

	}

	void Update() {
		if (mIsActive) {
			Step();
		}
	}


	Element() {}

	Element(int x, int y, int width, int height) {
		mRect = Rect(x, y, width, height);
	}

};


class Canvas {
private:
	Element *mElements[ELEMENT_LIMIT] = { nullptr };
	ushort mUsedElements = 0;
public:

	bool mIsVisible = true;

	void AddElement(Element* in, string_static newID = nullptr) {
		if (newID) { //If we gave a name
			in->ID = newID;
		}
		else {
			char newP[5] = "";
			SDL_snprintf(newP, 5, "ID%d", mUsedElements);
			in->ID = newP;
		}
		mElements[mUsedElements] = in;
		
		mUsedElements++;
		if (mUsedElements > ELEMENT_LIMIT) {

		}
	}

	Element* GetElementByID(string_static ID) {
		for (ushort i = 0; i < mUsedElements; i++) {
			if (!SDL_strcmp(mElements[i]->ID, ID)) {
				return mElements[i];
			}
		}
		return nullptr;
	}

	void UpdateElements();

	void RenderElements();

	Canvas();
};



extern Canvas* gCurrentCanvas;


#pragma region Templates
class eButton : public Element {
public:
	string_editable pText = nullptr;

	void Render() override {
		uRenderRect();
		uRenderText(pText);
	}
};

class eText : public Element {
public:
	string_editable pText = nullptr;

	void Render() override {
		uRenderText(pText);
	}
};

class eBar : public Element {
public:
	float pMinimum = 0.0f;
	float pCurrent = 50.0f;
	float pMaximum = 100.0f;
	Color pBarColor = { 0, 255, 0, 255 };
	void Render() override {
		uRenderRect();

		float percentage = static_cast<float>(static_cast<float>(pCurrent) / static_cast<float>(pMaximum));



		Rect bar = {
			mRect.x,
			static_cast<int>(static_cast<float>((mRect.y + mRect.h)) - static_cast<float>(mRect.h * percentage)),
			mRect.w,
			static_cast<int>(static_cast<float>(mRect.h) * static_cast<float>(percentage)),
		};
		gEzRender->cRenderRect(bar, true, &pBarColor);

	}

	//Runs when active!
	void OnClicked() override {
		float mouse = GetMousePos().y;
        SDL_Log("Mouse click pos: %f", mouse);
		float offset = mouse - static_cast<float>(mRect.x);
		SDL_Log("Offset: %f", offset);
		pCurrent = 100 - ((offset / static_cast<float>(mRect.h)) * 100);
		SDL_Log("As percentage: %f", pCurrent);
	}


	eBar() {
		mIsActive = false;
	}
};
#pragma endregion



#endif
