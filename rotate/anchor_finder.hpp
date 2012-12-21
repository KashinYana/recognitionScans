#ifndef __ANCHOR_FINDER_HPP__
#define __ANCHOR_FINDER_HPP__

#include "anchor.hpp"
#include <cv.h>
#include <exception>
#include <iostream>

class AnchorNotFoundException: public std::exception
{
	public:
		explicit AnchorNotFoundException(char* str): message(str) {
		}
		explicit AnchorNotFoundException(const std::string& msg = ""): message(msg) {
		}
		virtual const char* what() const throw()
		{
			return message.c_str();
		}
		virtual ~AnchorNotFoundException() throw() 
		{	
		}
	private:
		std::string message;
		
};

class AnchorFinder { 
private: 
	struct AnchorConfig {
		int maxAcceptedPixelDeviation; 
	};
	
	AnchorConfig config;

public:
	AnchorFinder(Config cfg) {
		try {
			config.maxAcceptedPixelDeviation = util::StrToInt(cfg.getSection("anchor_finder").getProperty("max_pixel_deviation"));
		} catch (std::exception e) {
			config.maxAcceptedPixelDeviation = 200000;
		}
	}
	
	std::vector<cv::Point> find(cv::Mat image, cv::Mat anchor, int count)	
	{
		if (!(anchor.rows <= image.rows && image.cols <= image.cols))
			throw std::logic_error("Anchor should be less than image");
		cv::Mat result;
		cv::matchTemplate(image, anchor, result, CV_TM_SQDIFF);	
		std::vector<cv::Point> answer;
		double minValue;
		cv::Point optimalPosition;	
		for (int i = 0; i < count; i ++)
		{
			cv::minMaxLoc(result, &minValue, NULL, &optimalPosition, NULL); 
			double average_value = minValue / (anchor.rows * anchor.cols);
			if (average_value > config.maxAcceptedPixelDeviation) {
				throw AnchorNotFoundException("Expected maxDeviation " + util::IntToStr(config.maxAcceptedPixelDeviation) + 
						" but " + util::IntToStr(average_value) + " got");
			}
			// we found left upper corner, let's find center
			optimalPosition.x += anchor.cols / 2; 
			optimalPosition.y += anchor.rows / 2;
			answer.push_back(optimalPosition);
			for (int i = std::max(optimalPosition.y - anchor.rows, 0); 				
				i <= std::min(optimalPosition.y + anchor.rows, image.rows - 1); i ++)
				for (int j = std::max(optimalPosition.x - anchor.cols, 0); 
					j <= std::min(optimalPosition.x + anchor.cols, image.cols - 1); j ++)
					{
						result.at<float>(i,j) = 1e100;		
					}
		}
		return answer;
	}

	cv::Point find(cv::Mat image, cv::Mat anchor) {
		std::vector<cv::Point> result = find(image, anchor, 1);
		return result[0];
	}
};

#endif
