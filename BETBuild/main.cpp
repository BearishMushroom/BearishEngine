#include <stdio.h>
#include "BETFile.h"
#include <SOIL.h>

int main(int argc, char** argv) {
	if (argc < 3) {
		// No input was specified.
		printf("Usage: BETBuild input.png output.bet");
		return -1;
	}

	std::string inputPath = std::string(argv[1]);
	std::string outputPath = std::string(argv[2]);

	int width, height;
	unsigned char* image = SOIL_load_image(inputPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA | SOIL_FLAG_INVERT_Y);

	Bearish::Core::BET::BETFile output;
	output.data = image;
	output.dataSize = width * height * 4;
	output.width = width;
	output.height = height;
	output.ConvertToBET();

	output.WriteToFile(outputPath);
	return 0;
}