#include <cv.h>
#include <highgui.h>
#include "../config/config.hpp"
#include "../images/image.hpp"
#include "anchor_rotater.hpp"
#include "../util/util.hpp"
#include <iostream>
#include <fstream>

cv::Vec3b RED = cv::Vec3b(0, 0, 255);
int delta = 15;

int main(int argc, char* argv[]) {
	Config config = Config::parse("config_for_rotater.cfg");
	if (argc == 1) {
		std::cerr << "Provide file to scan" << std::endl;
		return 1;
	}
	for (int i = 1; i < argc; i ++) {
		std::string fileToScan = argv[i];
		Image::Image image = Image::readImage(fileToScan);
		AnchorRotater anchorRotater(config);
		Image::Image new_image = anchorRotater.rotate(image);
		std::cerr << "Image " << fileToScan << " OK" << std::endl;
		Image::show(new_image);
	}
}
