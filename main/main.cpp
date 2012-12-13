#include "../images/image.hpp"
#include "../config/config.hpp"
#include "../rotate/anchor_rotater.hpp"
#include "../thresholding/thresholding.hpp"
#include "splitter.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cerr << "Provide file to process" << std::endl;
		return 1;
	}
	std::string config_file = "config.cfg"; // TODO: pass config file as command-line parameter
	std::string image_file = argv[1];
	Image::Image image = Image::read(image_file);
	Config config = Config::parse(config_file);
	AnchorRotater anchorRotater(config);
	std::cerr << image.rows << " " << image.cols << std::endl;
	image = anchorRotater.rotate(image);
	std::cerr << image.rows << " " << image.cols << std::endl;
//	image = Thresholding(image).transformation();
	Image::show(image);
	std::vector<cv::Mat> pieces = Splitter(config).split(image);
	for (int i = 0; i < pieces.size(); i ++) {
		std::cerr << pieces[i].rows << " " << pieces[i].cols << std::endl;
		Image::show(pieces[i]);
	}
	return 0;
}
