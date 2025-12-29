#pragma once

#include "common.h"


constexpr byte MAX_CELLS = 12;

struct Cell {
	ushort width = 0xFFFF;
	ushort height = 0xFFFF;
	byte frame_delay = 0xFF;
	byte frame_amount = 0xFF;
};

struct Sprite {
	SDL_Texture* mTexturePage = nullptr;

	Cell* mCells = nullptr;
	byte mCellIndex = 0;


	byte mFrame_index = 0;
	ushort mFrame_delay = 0;


	void Reset() {
		mFrame_index = 0;
		mFrame_delay = 0;
		mCellIndex = 0;
	}

	void SetCellIndex(byte pNewIndex) {
		Reset();
		mFrame_index = pNewIndex;
	}

	void Update() {
		Cell& cur = mCells[mCellIndex];
		if (cur.frame_amount == 1) return;
		mFrame_delay++;
		if (mFrame_delay >= cur.frame_delay) {
			mFrame_delay = 0;
			mFrame_index++;
			if (mFrame_index >= cur.frame_amount) {
				mFrame_index = 0;

			}
		}
	}


	Sprite() {}
};