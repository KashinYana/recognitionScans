#ifndef __INTERESTING_OBJECT_HPP__
#define __INTERESTING_OBJECT_HPP__

#include "../images/image.hpp"

struct InterestingObject {
	cv::Mat image;
	int x, y, width, height;
	std::string recognitionResult;
};


#endif
