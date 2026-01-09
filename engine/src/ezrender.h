#pragma once

#include "common.h"
#if !COMOPT_R_USE_HA
#include "instance.h"
#endif

#include "sprite.h"
#include "object.h"

/* This renderer is made to make rendering easier.
* So if you don't want rendering, then its done!
* If you don't want to use SDL, then its done!

*/

struct Font {
#if COMOPT_R_USE_HA
	SDL_Texture* image = nullptr;
#else
	SDL_Surface* image = nullptr;
#endif
	byte usageWidth = 0;
	byte usageHeight = 0;
	//bool sHasNumbers = false;
	//bool sHasLowercase = false;

};

class EzRender {
private:

public:
#if COMOPT_R_USE_HA
	SDL_Renderer* mRenderer = nullptr;
#else
	Color mRenderColor;
#endif

	void RenderClear() {
#if COMOPT_R_USE_HA
		cSetDrawColor(0, 255, 0, 255);
		SDL_RenderClear(mRenderer);
#else
		SDL_ClearSurface(SDL_GetWindowSurface(gInstance->mWindow), 0, 255, 0, 255);
#endif
	}

	void RenderStart() {
		RenderClear();
	}

	void RenderEnd() {
#if COMOPT_R_USE_HA
		SDL_RenderPresent(mRenderer);
#else
		SDL_UpdateWindowSurface(gInstance->mWindow);
#endif
	}


	void cSetDrawColor(byte r = 0, byte g = 0, byte b = 0, byte a = 255) {
#if COMOPT_R_USE_HA
		SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
#else
		//mRenderColor = Color(r, g, b, a);
		Color c = {
			r,
			g,
			b,
			a
		};
		mRenderColor = c;
#endif
	}

	void cSetDrawColor(Color c) {
#if COMOPT_R_USE_HA
		SDL_SetRenderDrawColor(mRenderer, c.r, c.g, c.b, c.a);
#else
		mRenderColor = c;
#endif
	}

	void cRenderRect(Rect r, bool isFilled = false, Color* c = nullptr) {
		if (c) {
			cSetDrawColor(*c);
			delete c;
		}

#if COMOPT_R_USE_HA
		SDL_FRect compilerFix = r;
		if (isFilled) {
			
			SDL_RenderFillRect(mRenderer, &compilerFix);
		}
		else SDL_RenderRect(mRenderer, &compilerFix);
#else
		if (isFilled) {
			SDL_Surface* s = SDL_CreateSurface(r.w, r.h, SDL_PIXELFORMAT_RGBA8888);
			SDL_Rect rR = r;
			Uint32 color = SDL_MapRGB(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888), NULL, mRenderColor.r, mRenderColor.g, mRenderColor.b);


			SDL_FillSurfaceRect(SDL_GetWindowSurface(gInstance->mWindow), &rR, color);

			//SDL_BlitSurface(s, NULL, SDL_GetWindowSurface(gInstance->mWindow), &rR);
		}
#endif
	}
#if COMOPT_R_USE_HA
	void cRenderTexture(SDL_Texture* in, Rect crop, Rect position, float rotation = 0.0f, SDL_FlipMode flipMode = SDL_FLIP_NONE) {
		SDL_FRect compilerFix_crop = crop;
		SDL_FRect compilerFix_position = position;
		SDL_RenderTextureRotated(mRenderer, in, &compilerFix_crop, &compilerFix_position, rotation, NULL, flipMode);
	}
#else
	void cRenderTexture(SDL_Surface* in, Rect crop, Rect position, float rotation = 0.0f, SDL_FlipMode flipMode = SDL_FLIP_NONE) {
		SDL_FlipSurface(in, flipMode);
		//SDL_RotateSurface(); We need to wait for SDL 3.4.0, we are on 3.2.2
		SDL_RotateSurface(in, rotation);
		SDL_FlipSurface(in, flipMode);
		SDL_Rect cropR = crop;
		SDL_Rect positionR = position;
		SDL_BlitSurface(in, &cropR, SDL_GetWindowSurface(gInstance->mWindow), &positionR);
	}
#endif

	void cRenderText(float x, float y, string_static format, ...) {
#if COMOPT_R_USE_HA
		cSetDrawColor(0, 0, 0, 255);
		SDL_RenderDebugTextFormat(mRenderer, x, y, format);
#else

#endif
	}

	void cRenderObject(Vector2 pCameraPos, MaxObject *pObject) {
		Rect transAsRect = {
			static_cast<int>(pObject->mTransform.position.x - pCameraPos.x),
			static_cast<int>(pObject->mTransform.position.y - pCameraPos.y),
			static_cast<int>(pObject->mTransform.width),
			static_cast<int>(pObject->mTransform.height),
		};
#if COMOPT_R_USE_HA
		SDL_FRect compilerFix = transAsRect;
#endif


		if (pObject->mUseSprite) {
			Sprite& s = pObject->mSprite;
			Cell& sC = s.mCells[s.mCellIndex];


			ushort textureOffset = 0;
			Rect texPos = {
				static_cast<int>(sC.width * s.mFrame_index),
				static_cast<int>(textureOffset),
				static_cast<int>(sC.width),
				static_cast<int>(sC.height),
			};

#if COMOPT_R_USE_HA
			if (s.mCellIndex > 0) {
				for (short i = s.mCellIndex - 1; i >= 0; i--) {
					textureOffset += s.mCells[i].height;
				}
			}


			cRenderTexture(s.mTexturePage, texPos, transAsRect, pObject->mTransform.rotation, (pObject->mTransform.direction == 1 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL));
			//cRenderTexture(mRenderer, pObject->mSprite.mTexturePage, &texPos, &compilerFix, pObject->mTransform.rotation, NULL, (pObject->mTransform.direction == 1 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL) );
#else

			cRenderTexture(s.mTexturePage, texPos, transAsRect);
#endif
		}

		else {
			cSetDrawColor(255, 0, 0, 255);
			cRenderRect(transAsRect, true);
		}

	}

	void cRenderTileset() {

	}

	void cRenderText(Font font, int x, int y, string_static text, byte size = 1) {
		char curIndex = 0;
		byte yClipPos = 0;

		int renderX = x;
		for (ushort i = 0; i < SDL_strlen(text); i++) {
			

			switch (text[i]) {
				case ' ':
					renderX += font.usageWidth * size;
					continue;
				break;
				
				case '\n':
					renderX = 0;
					y += font.usageHeight * size;
					continue;
				break;

				case '?':
					curIndex = 10;
					yClipPos = font.usageHeight * 2;

					break;
				case '!':
					curIndex = 11;
					yClipPos = font.usageHeight * 2;
				break;

				case '.':
					curIndex = 12;
					yClipPos = font.usageHeight * 2;
				break;

				case ',':
					curIndex = 13;
					yClipPos = font.usageHeight * 2;
				break;

				default:
					if (text[i] >= 'A' && text[i] <= 'Z') { //Check if we are in the alphabet
						yClipPos = 0;
						curIndex = text[i] - 'A'; //a - a = 0, b - a = 1
					}
					else if (text[i] >= 'a' && text[i] <= 'z') {
						yClipPos = font.usageHeight;
						curIndex = text[i] - 'a'; //a - a = 0, b - a = 1
						//SDL_Log("Char: %c\nIndex: %d", text[i], curIndex);
					}
					else if (text[i] >= '0' && text[i] <= '9') {
						yClipPos = font.usageHeight * 2;
						curIndex = text[i] - '0';
					}
				break;
			}
			
			Rect renderPos = Rect(x + renderX, y, font.usageWidth * size, font.usageHeight * size);
			//cSetDrawColor(255, 255, 0, 255);
			//cRenderRect(renderPos, true);
			cRenderTexture(font.image, Rect(curIndex * font.usageWidth, yClipPos, font.usageWidth, font.usageHeight ), renderPos);
			renderX += font.usageWidth * size;
		}
	}
	


	EzRender();
};


extern EzRender* gEzRender;