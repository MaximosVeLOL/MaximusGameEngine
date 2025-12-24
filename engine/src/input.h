#pragma once

#include "common.h"

constexpr byte MAX_PLAYERS = 4;

enum InputCode {
	INPUT_CODE_UP = 0,
	INPUT_CODE_DOWN,
	INPUT_CODE_LEFT,
	INPUT_CODE_RIGHT,
	INPUT_CODE_LAST
};

struct Input {
	SDL_Scancode mKeyboard = SDL_SCANCODE_UNKNOWN;
	SDL_GamepadButton mGamepad = SDL_GAMEPAD_BUTTON_INVALID;

	Input(SDL_Scancode pCode, SDL_GamepadButton pPad = SDL_GAMEPAD_BUTTON_INVALID) : mKeyboard(pCode), mGamepad(pPad) {}
};

struct InputState {
	bool prev = false;
	bool cur = false;
};


extern Input mInputs[INPUT_CODE_LAST];

class InputSystem {
private:
	InputState mStates[MAX_PLAYERS][INPUT_CODE_LAST] = {false};
	const bool* mKeyboardState = nullptr;
public:

	float mMouseX = 0;
	float mMouseY = 0;
	byte mMouseState = SDL_BUTTON_LEFT;
	byte mPreviousMouseState = SDL_BUTTON_LEFT;

	Vector2 GetMousePos() {
		return Vector2(mMouseX, mMouseY);
	}

	bool GetMouseDown(SDL_MouseButtonFlags pWhichButton) {
		return mMouseState & SDL_BUTTON_MASK(pWhichButton) && mPreviousMouseState != mMouseState;
	}

	bool GetMouseHeld(SDL_MouseButtonFlags pWhichButton) {
		return mMouseState & SDL_BUTTON_MASK(pWhichButton);
	}

	void UpdateInput(InputCode code, byte layer = 0) {
		InputState& s = mStates[layer][code];
		s.prev = s.cur;
		s.cur = mKeyboardState[mInputs[code].mKeyboard];
		//s.cur = SDL_GetGamepadButton();
	}

	bool GetInputUp(InputCode code, byte layer = 0) {
		InputState& cur = mStates[layer][code];
		return (!cur.cur);
	}
	bool GetInputDown(InputCode code, byte layer = 0) {
		InputState& cur = mStates[layer][code];
		return (!cur.prev && cur.cur);
	}
	bool GetInputHeld(InputCode code, byte layer = 0) {
		InputState& cur = mStates[layer][code];
		return (cur.cur);
	}
	bool GetInputReleased(InputCode code, byte layer = 0) {
		InputState& cur = mStates[layer][code];
		return (cur.prev && !cur.cur);
	}

	void UpdateAll() {
		mPreviousMouseState = mMouseState;
		mMouseState = SDL_GetMouseState(&mMouseX, &mMouseX);

		mKeyboardState = SDL_GetKeyboardState(NULL);
		for (byte layerI = 0; layerI < MAX_PLAYERS; layerI++) {
			for (ushort inputI = 0; inputI < INPUT_CODE_LAST; inputI++) {
				UpdateInput((InputCode)inputI, layerI);
			}
		}
	}

	InputSystem();
};

extern InputSystem* gInput;