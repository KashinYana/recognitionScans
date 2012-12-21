#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

#include "../../../images/image.hpp"
#include "../../../util/util.hpp"
#include "../processor.hpp"

const int IMAGE_SIZE = 28;

const int ESC_KEY = 27;

int image_count = 0;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: file-from file-to" << std::endl;	
		return 1;
	}
	cv::Mat image = Image::read(argv[1]);
	Image::write(argv[2], ImageProcessor(image).process(IMAGE_SIZE));
}
