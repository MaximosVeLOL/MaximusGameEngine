#include "src/util.h"

int main(int argc, char *argv[]) {
	if (argc < 5) {
		printf("Usage: (sprite.png) (cel width) (frame delay) (texture page name no extension)");
		return -1;
	}
	CreateSpriteFile(argv[1], atoi(argv[2]), atoi(argv[3]), argv[4]);
	return 1;
}
