/*
#ifndef __MGUI_H__
#define __MGUI_H__

#include "common.h"
#include "ezrender.h"
#include "input.h"

constexpr ushort ELEMENT_LIMIT = 100;



namespace MGUI {
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

	bool MouseIsOver(Rect rect) {
		Vector2 m = gInput->GetMousePos();
		return m.x >= rect.x && m.x <= rect.x + rect.w
			&& m.y > +rect.y && m.y <= rect.y + rect.h;
	}


	class Element {
	public:
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
			if (mVisible) {
				Render();
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

		void AddElement(Element* in) {
			mElements[mUsedElements] = in;
			mUsedElements++;
			if (mUsedElements > ELEMENT_LIMIT) {

			}
		}

		void UpdateElements() {
			for (ushort i = 0; i < mUsedElements; i++) {
				Element& cur = *mElements[i];
				if (MouseIsOver(cur.mRect)) {
					cur.mMouseOver = true;
					if (cur.mIsActive) {
						cur.mRenderColor = STYLE.backgroundHighlighted;
						if (gInput->GetMouseDown(SDL_BUTTON_LEFT)) {
							cur.OnClicked();

						}
					}
				}
				else {
					cur.mRenderColor = STYLE.background;
					cur.mMouseOver = false;
				}
				cur.Update();
				
			}
		}

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

			float percentage = (pCurrent / pMaximum);

			Rect bar = {
				mRect.x,
				static_cast<float>((mRect.y + mRect.h)) - static_cast<float>(mRect.h * percentage),
				mRect.w,
				static_cast<float>(mRect.h) * static_cast<float>(percentage),
			};
			gEzRender->cRenderRect(bar, true, &pBarColor);

		}

		eBar() {
			mIsActive = false;
		}
	};
#pragma endregion
}


#endif
*/