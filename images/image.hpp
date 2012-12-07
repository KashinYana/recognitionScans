#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <cv.h>
#include <highgui.h>
#include <stdexcept>

namespace Image {

	typedef cv::Mat Image;

	cv::Mat readImage(const std::string& fileName) {
		cv::Mat result = cv::imread(fileName);
		if (result.data == NULL) {
			throw std::invalid_argument("Failed to read image from file " + fileName);
		}
		return result;
	}

	cv::Mat read(const std::string& fileName) {
		return readImage(fileName);
	}

	void show(const Image& image) {
		cv::namedWindow("window", CV_WINDOW_NORMAL);
		cv::imshow("window", image);
		cv::waitKey();
	}
}

#endif
