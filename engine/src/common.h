#ifndef __COMMON_H__
#define __COMMON_H__

#include <SDL3/SDL.h>
#include <string>

#pragma region Compiler options

//Use hardware acceleration
#define COMOPT_R_USE_HA 1
#define COMOPT_R_USE 1
#define COMOPT_C_CLIENT 1
#define COMOPT_C_USE_ASSET 1

#pragma endregion

#pragma region Extra Settings

#define RENDER_WIDTH 960
#define RENDER_HEIGHT 540

#pragma endregion

#define ERROR(message) \
	SDL_Log(message); \
	return


typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong; //A regular long is an int.

typedef const char* string_static;
typedef char* string_editable;
typedef std::string string_easy;
#define STATIC_AS_EDITABLE(message) (char *)message

//Non SDL-specific things


struct Color {
	byte r = 0;
	byte g = 0;
	byte b = 0;
	byte a = 0;
	
	//Color() {}
	//Color(byte pR = 0, byte pG = 0, byte pB = 0, byte pA = 255) : r(pR), g(pG), b(pB), a(pA) {}
};






template<class T>
struct TRect {
	T x = T();
	T y = T();
	T w = T();
	T h = T();

	TRect() {}
	TRect(T pX, T pY, T pW, T pH) : x(pX), y(pY), w(pW), h(pH) {}

	operator SDL_Rect() {
		SDL_Rect r = {
			static_cast<int>(x),
			static_cast<int>(y),
			static_cast<int>(w),
			static_cast<int>(h),
		};
		return r;
	}
	operator SDL_FRect() {
		SDL_FRect r = {
			static_cast<float>(x),
			static_cast<float>(y),
			static_cast<float>(w),
			static_cast<float>(h),
		};
		return r;
	}

};

typedef TRect<int> Rect;
typedef TRect<float> FRect;
typedef TRect<short> SRect;
typedef TRect<ushort> GUIRect;

template<class T>
struct TVector2 {
	T x = T();
	T y = T();

	TVector2 operator+(TVector2 other) {
		return TVector2<T>(x + other.x, y + other.y);
	}

	TVector2() {}
	TVector2(T pX, T pY) : x(pX), y(pY) {}
};
typedef TVector2<float> Vector2;
typedef TVector2<int> Vector2Int;
typedef TVector2<short> Vector2Short;


#endif