#include "input.h"



Input mInputs[INPUT_CODE_LAST] = {
	Input(SDL_SCANCODE_UP, SDL_GAMEPAD_BUTTON_DPAD_UP),
	Input(SDL_SCANCODE_DOWN, SDL_GAMEPAD_BUTTON_DPAD_DOWN),
	Input(SDL_SCANCODE_LEFT, SDL_GAMEPAD_BUTTON_DPAD_LEFT),
	Input(SDL_SCANCODE_RIGHT, SDL_GAMEPAD_BUTTON_DPAD_RIGHT),

};




InputSystem* gInput = nullptr;


InputSystem::InputSystem() {
	gInput = this;
}