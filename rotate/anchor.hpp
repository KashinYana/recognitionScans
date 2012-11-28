#ifndef __ANCHOR_H__
#define __ANCHOR_H__

#include <cv.h>
#include <vector>

class Anchor {

public:
	cv::Mat image;	
	std::vector<cv::Point> positions;

	Anchor(cv::Mat _image, cv::Point _position): image(_image), positions(1, _position)														
	{		
	}

	Anchor(cv::Mat _image, int x, int y): image(_image), positions(1, cv::Point(x, y)) 
	{
	}

	Anchor(cv::Mat _image, std::vector <cv::Point> _positions): image(_image), positions(_positions)
	{
	}
};

#endif // __ANCHOR_H__
