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

	void write(const std::string& fileName, cv::Mat image) {
		cv::imwrite(fileName, image);
	}

	int show(const Image& image) {
		cv::namedWindow("window", CV_WINDOW_NORMAL);
		cv::imshow("window", image);
		return cv::waitKey();
	}

	cv::Mat makeBlackAndWhite(cv::Mat image) {
		cv::Mat result(image.rows, image.cols, CV_8UC1);
		cvtColor(image, result, CV_RGB2GRAY);
		return result;
	}

	cv::Mat copy(cv::Mat image) {
		cv::Mat result;
		image.copyTo(result);
		return result;
	}

	cv::Mat getSubMatrix(cv::Mat image, int x, int y, int width, int height) {
		return image.rowRange(y, y + height).colRange(x, x + width).clone();
	}
}

#endif
