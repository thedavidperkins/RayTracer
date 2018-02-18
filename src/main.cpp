#include "InputRead.h"
#include "RayTracer.h"
#include <FreeImage.h>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Error: must supply a filename argument" << std::endl;
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		RayTracer rt(10);
		if (!readFile(argv[i], rt)) {
			return 1;
		}
		rt.computePixels();

		FreeImage_Initialise();
		FIBITMAP* img = FreeImage_ConvertFromRawBits(rt.getPixels(), rt.getWidth(), rt.getHeight(), rt.getWidth() * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
		std::string outFile = rt.outFileName == "" ? "out.png" : rt.outFileName;
		std::cout << "saving to " << outFile << std::endl;
		FreeImage_Save(FIF_PNG, img, outFile.c_str(), 0);
		FreeImage_DeInitialise();
	}
}