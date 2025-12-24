#pragma once

#include "common.h"
#include "file.h"


#define USE_PNG true


#if USE_PNG
#define LOAD_IMAGE(source, closeStream) SDL_LoadPNG_IO(source, closeStream)
#define SAVE_IMAGE(surface, file) SDL_SavePNG(surface, file)
#else
#define LOAD_IMAGE(source, closeStream) SDL_LoadBMP_IO(source , closeStream)
#define SAVE_IMAGE(surface, file) SDL_SaveBMP(surface, file)
#endif




void CreateSpriteFile(string_static pSpriteImage, ushort pCelWidth, byte pFrameDelay, string_static pTexturePageNameNoExtension) {
	File spr_file(pSpriteImage);
	SDL_Surface* spr_surface = LOAD_IMAGE(spr_file, false);
	if (!spr_surface) {
		ERROR("Failed to open sprite image!");
	}
#if USE_PNG
	File tex_file("%s.png", pTexturePageNameNoExtension); //texturepage.bmp
#else
	File tex_file("%s.bmp", pTexturePageNameNoExtension); //texturepage.bmp
#endif
	if (!tex_file.mExisted) { //If we don't exist, make one! Else, add to it!
		void* v = spr_file.ReadAll();
		tex_file.Write(v);
		tex_file.Close();
	}
	else {
		
		SDL_Surface* tex_surface = LOAD_IMAGE(tex_file, false);
		if (!tex_surface) {
			ERROR("Failed to open texture page image!");
		}
		SDL_Surface* output = SDL_CreateSurface((spr_surface->w > tex_surface->w ? spr_surface->w : tex_surface->w), tex_surface->h + spr_surface->h, SDL_PIXELFORMAT_RGBA8888);
		if (!output) {
			ERROR("Failed to create output surface!");
		}
		SDL_Rect tex_pos = {
			0,
			0,
			tex_surface->w,
			tex_surface->h
		};
		SDL_Rect spr_pos = {
			0,
			tex_surface->h,
			spr_surface->w,
			spr_surface->h
		};
		//Put the texture page on the new surface
		SDL_BlitSurface(tex_surface, &tex_pos, output, &tex_pos);
		SDL_BlitSurface(spr_surface, &spr_pos, output, &spr_pos);
		

		SAVE_IMAGE(output, tex_file, false);
		
		SDL_DestroySurface(output);

		SDL_DestroySurface(tex_surface);
	}

	
	File texpage_file(o_append, "%s.MESF", pTexturePageNameNoExtension);
	/* Sprite format:
		0 - 4 = Header (MESF)

		Usage width (short)
		Usage height (short)
		Frame delay (byte)
		Amount of frames (byte)
		
	*/

	if (!texpage_file.mExisted)
		texpage_file.Write("MESF");
	//Width
	texpage_file.Write(pCelWidth);
	//Height
	texpage_file.Write((ushort)spr_surface->h);
	//Frame delay
	texpage_file.Write(pFrameDelay);
	//Frame amount
	texpage_file.Write((byte)SDL_lroundf(spr_surface->w / pCelWidth));

	texpage_file.Close();
	SDL_DestroySurface(spr_surface);
	tex_file.Close();
	spr_file.Close();
}