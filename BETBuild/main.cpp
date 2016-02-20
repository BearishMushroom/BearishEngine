#include <stdio.h>
#include "BETFile.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		// No input was specified.
		printf("Usage: BETBuild input.png output.bet");
		return -1;
	}

	string inputPath = string(argv[1]);
	string outputPath = string(argv[2]);

	i32 width, height;
	u8* image = SOIL_load_image(inputPath.c_str(), &width, &height, 0, 0);

	BETFile output;
	output.data = image;
	output.dataSize = width * height * 4;
	output.width = width;
	output.height = height;
	output.ConvertToBET();

	output.WriteToFile(outputPath);

	output.ConvertToRaw();
	SOIL_save_image("test.tga", SOIL_SAVE_TYPE_TGA, output.width, output.height, 4, output.data);

	return 0;
}