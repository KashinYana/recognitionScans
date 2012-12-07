#include <cv.h>
#include <highgui.h>
#include "../config/config.hpp"
#include "../images/image.hpp"
#include "anchor_finder.hpp"
#include "../util/util.hpp"
#include <iostream>
#include <fstream>

cv::Vec3b RED = cv::Vec3b(0, 0, 255);
int delta = 5;

int main(int argc, char* argv[]) {
	Config config = Config::parse("config_for_finder.cfg");
	std::string anchorFile = config.getSection("anchor").getProperty("file");
	if (argc == 1) {
		std::cerr << "Provide file to scan" << std::endl;
		return 1;
	}
	std::string fileToScan = argv[1];
	Image::Image anchor = Image::readImage(anchorFile);
	Image::Image image = Image::readImage(fileToScan);
	AnchorFinder anchorFinder(config);
	int anchorCount = util::StrToInt(config.getSection("anchor").getProperty("count"));
	std::vector<cv::Point> positions = anchorFinder.find(image, anchor, anchorCount);
	delta = std::max(delta, std::min(image.rows, image.cols) / 100);
	std::cout << "size = (" << image.rows << ", " << image.cols << ")" << std::endl;
	for (int k = 0; k < anchorCount; k ++) {
		std::cout << "position = (" << positions[k].x << ", " << positions[k].y << ")" << std::endl;
		for (int i = -delta; i <= delta; i ++)
			for (int j = -delta; j <= delta; j ++)
				image.at<cv::Vec3b>(i + positions[k].y, j + positions[k].x) = RED;
	}
	cv::namedWindow("window", CV_WINDOW_NORMAL);
	cv::imshow("window", image);
	cv::waitKey();
}
