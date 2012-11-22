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

class AnchorFounder { 
private: 
	struct AnchorConfig {
		int maxAcceptedPixelDeviation; 
	};
	AnchorConfig config;

	int StrToInt(const std::string& s) {
		int result = 0;
		int pos = 0;
		int sign = 1;
		if (s[pos] == '-') {
			sign *= -1;
			pos ++;
		}
		if (pos == 0 && s[pos] == '+') {
			pos ++;
		}
		for (int i = pos; i < s.size(); i ++) {
			if (isdigit(s[i])) {
				value = 10 * value + s[i] - '0';
			} else {
				throw std::illegal_argument("expected digit, but " + (string)s[i] + " got");
			}
		}
	}

	std::string IntToStr(int value) {
		std::string result = "";
		if (value < 0) { 
			result += '-';
			value = -result;
		}
		if (value == 0) {
			return "0";
		}
		while (value > 0) {
			result += value % 10 + '0';
			value /= 10;
		}
		return result;
	}
		
public:
	AnchorFounder(Config cfg) {
		unsigned int entries = cfg["anchor"]["max_pixel_deviation"].size();
		if (entries == 0) {
			throw std::illegal_argument("Expected definition of max_pixel_deviation in config");
		}
		if (entries > 1) {
			// warning("Multiply definition of max_pixel_deviation in config")
		}
		config.maxAcceptedPixelDeviation = StrToInt(*cfg["anchor"]["max_pixel_deviation"]);
	}

	std::vector<cv::Point> findAnchor(cv::Mat image, const Anchor& anchor)	
	{
		if (!(anchor.image.rows <= image.rows && anchor.image.cols <= image.cols))
			throw std::logic_error("Anchor should be less than image");
		cv::Mat result;
		cv::matchTemplate(image, anchor.image, result, CV_TM_SQDIFF);	
		std::vector<cv::Point> answer;
		double minValue;
		cv::Point optimalPosition;	
		for (int i = 0; i < anchor.positions.size(); i ++)
		{
			cv::minMaxLoc(result, &minValue, NULL, &optimalPosition, NULL); 
			double average_value = minValue / (anchor.image.rows * anchor.image.cols);
			if (average_value > maxAcceptedPixelDeviation) {
				throw AnchorNotFoundException("Expected maxDeviation " + IntToStr(maxAcceptedPixelDeviation) + 
						" but " + IntToStr(average_value) + " got");
			}
			// we found left upper corner, let's find center
			optimalPosition.x += anchor.image.cols / 2; 
			optimalPosition.y += anchor.image.rows / 2;
			answer.push_back(optimalPosition);
			for (int i = std::max(optimalPosition.y - anchor.image.cols, 0); 				
				i <= std::min(optimalPosition.y + anchor.image.cols, image.cols - 1); i ++)
				for (int j = std::max(optimalPosition.x - anchor.image.rows, 0); 
					j <= std::min(optimalPosition.x + anchor.image.rows, image.rows - 1); j ++)
						result.at<float>(i,j) = 1e100;		
	}
	return answer;
}

private:
	int distance(const cv::Point& from, const cv::Point& to)
	{
		return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
	}

	void normalizeDistance(std::vector<cv::Point>& points) 
	{
		int minDistance = -1;
		int point1, point2;
		for (unsigned int i = 0; i < points.size(); i ++)
			for (unsigned int j = i + 1; j < points.size(); j ++)
				if (distance(points[i], points[j]) < minDistance || minDistance == -1)
				{
					minDistance = distance(points[i], points[j]);
					point1 = i;
					point2 = j;
				}
		std::swap(points[0], points[point1]);
		std::swap(points[1], points[point2]);
		minDistance = -1;
		int point = 0;
		for (unsigned int i = 2; i < points.size(); i ++)
			for (unsigned int j = 0; j <= 1; j ++)
				if (distance(points[i], points[j]) < minDistance || minDistance == -1)
				{
					minDistance = distance(points[i], points[j]);
					point = j;
			}
			if (point == 1)
		{
			std::swap(points[0], points[1]);
		}
	}

public:

	cv::Mat rotateByAnchors(const cv::Mat& image, const std::vector<Anchor>& anchors) 
	{
		const int baseAnchors = 3;
		if (anchors.size() < 1)
			throw std::logic_error("There should be at least 1 anchor");
		if (anchors[0].positions.size() < baseAnchors)
			throw std::logic_error("First anchor used as base one, he should have at least 3 positions");
		std::vector<cv::Point> initialPoints(baseAnchors);	
		std::vector<cv::Point> finalPoints(baseAnchors);
		for (int i = 0; i < baseAnchors; i ++) 
			initialPoints = findAnchor(image, anchors[0]);
		for (int i = 0; i < baseAnchors; i ++)
			finalPoints[i] = anchors[0].positions[i];
		__impl__::normalizeDistance(initialPoints);
		__impl__::normalizeDistance(finalPoints);
		cv::Mat transformMatrix = cv::getAffineTransform(initialPoints, finalPoints);
		cv::Mat result;
		cv::Scalar WHITE_COLOR = cv::Scalar(255, 255, 255);
		cv::warpAffine(image, result, transformMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, WHITE_COLOR);
		return result;	
	}

