#include "assethandler.h"


DefaultAssets mDefault;
void InitializeDefaultTextures() {
	File f;
	f.OpenFileFormatted(o_read, "%s/invalid.png", GFX_DIR);
	mDefault.mTexture = SDL_LoadPNG_IO(f.mCurrent, false);
	f.Close();
	f.OpenFileFormatted(o_read, "%s/invalid.wav", GFX_DIR);
	mDefault.mAudio = MIX_LoadAudio_IO(gAudio->mMixer, f.mCurrent, true, false);
	f.Close();
	mDefault.mSprite = new Sprite();
	mDefault.mSprite->mTexturePage = SDL_CreateTextureFromSurface(gEzRender->mRenderer, mDefault.mTexture);
	mDefault.mSprite->mCells = new Cell[1];
	Cell* c = &mDefault.mSprite->mCells[0];
	c->width = 64;
	c->height = 64;
	c->frame_delay = 10;
	c->frame_amount = 2;
}

bool uFileExtensionEquals(string_static pFileName, string_static pTargetExtension) {
	char* pos_str = SDL_strstr(pFileName, ".");
	if (!pos_str) return false;
	byte pos_real = pos_str - pFileName;
	//SDL_Log("Dot position: %d", pos_real);
	for (byte i = 0; i < SDL_strlen(pTargetExtension); i++) {
		//SDL_Log("(%d) %c = %c", i, pFileName[pos_real + i], pTargetExtension[i]);
		if (pFileName[pos_real + i] != pTargetExtension[i]) return false;
	}

	return true;
}

class TestAssetGroup : public AssetGroup {
public:
	TestAssetGroup() {
		//SDL_Log("Initializing mAssets!");
		mAssets[0] = Asset("plr.MESF");
		mAssets[1] = Asset("bg.wav");
		//mAssets[1] = Asset("test.MESF");
		mAssetCount = 2;
	}
};

AssetGroup mGroups[ASSET_GROUP_LAST] = {
	AssetGroup(),
	TestAssetGroup(),

};

AssetHandler* gAssetHandler = nullptr;



AssetHandler::AssetHandler() {
	gAssetHandler = this;
	InitializeDefaultTextures();

}