#ifndef __ANCHOR_ROTATER_HPP__
#define __ANCHOR_ROTATER_HPP__

#include "../images/image.hpp"
#include "anchor_finder.hpp"
#include <exception>
#include <iostream>

class AnchorRotater { 
private: 
	Config config;
	AnchorFinder anchorFinder;

public:
	AnchorRotater(Config cfg): anchorFinder(cfg), config(cfg) {
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

	struct Anchor {
		cv::Mat image;
		int count;
		std::vector<cv::Point> positions;
		Anchor(cv::Mat img, int cnt, const std::vector<cv::Point>& p): image(img), count(cnt), positions(p) {
		}
	};

	std::vector<Anchor> getAnchors() {
		std::vector<Anchor> result;
		for (Config::Section section: config["anchor"]) {
			std::vector<cv::Point> positions;
			for (std::string value: section["position"]) {
				std::pair<int,int> pair = util::StrToPair(value);
				positions.push_back(cv::Point(pair.first, pair.second));
			}
			if (section.hasProperty("count")) {
				result.push_back(Anchor(Image::readImage(section.getProperty("file")), 
							util::StrToInt(section.getProperty("count")), 
							positions
							));
			} else {
				result.push_back(Anchor(Image::readImage(section.getProperty("file")), 1, positions));
			}
		}		
		return result;
	}

	std::vector<cv::Point2f> convertVector(const std::vector<cv::Point>& vector) {
		std::vector<cv::Point2f> result(vector.size());
		for (int i = 0; i < vector.size(); i ++) {
			result[i] = vector[i];
		}
		return result;
	}

	cv::Size getSize() {
		std::pair<int,int> pair = util::StrToPair(config.getSection("model_image").getProperty("size"));
		return cv::Size(pair.first, pair.second);
	}

public:

	cv::Mat rotate(const Image::Image& image) 
	{
		std::vector<Anchor> anchors = getAnchors();
		const int baseAnchors = 3;
		if (anchors.size() < 1)
			throw std::invalid_argument("There should be at least 1 anchor");
		if (anchors[0].count != baseAnchors)
			throw std::invalid_argument("Base anchor used as base one, he should have at least 3 positions");
		std::vector<cv::Point> initialPoints(baseAnchors);	
		std::vector<cv::Point> finalPoints(baseAnchors);
		initialPoints = anchorFinder.find(image, anchors[0].image, baseAnchors);
		int index = 0;
		for (int i = 0; i < baseAnchors; i ++)
			finalPoints[i] = anchors[0].positions[i];
		normalizeDistance(initialPoints);
		normalizeDistance(finalPoints);
		cv::Mat transformMatrix = cv::getAffineTransform(convertVector(initialPoints), convertVector(finalPoints));
		cv::Mat result;
		cv::Scalar WHITE_COLOR = cv::Scalar(255, 255, 255);
		cv::warpAffine(image, result, transformMatrix, getSize(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, WHITE_COLOR);
		return result;	
	}

};

#endif



