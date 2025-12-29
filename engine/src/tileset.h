#pragma once

#include "common.h"




/* Tilesets
	Total size is determined by the file
	Tile size is determined by the file

*/


struct Tile {
	byte mX = 0;
	byte mY = 0;
	byte mID = 0;
};

struct Tileset {
	byte mID = 0; //For use in other files
	Tile* mTiles = nullptr;
	SDL_Texture* mTilesetTexture;
	//Tileset info
	byte mTileW = 0; // times the tile size
	byte mTileH = 0; // times the tile size
	byte mTileSize = 0;
};