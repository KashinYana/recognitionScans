#ifndef __SPLITTER_H__
#define __SPLITTER_H__

#include "../config/config.hpp"
#include "../images/image.hpp"
#include "../util/util.hpp"
#include "interesting_object.hpp"

class Splitter {
  private:
	Config config;
	int oldX;
	int oldY;
	
	int getRealX(int x, cv::Size size) {
		return round(x / (double) oldX * size.width);
	}

	int getRealY(int y, cv::Size size) {
		return round(y / (double) oldY * size.height);
	}

  public:
	Splitter(Config cfg): config(cfg) {
		std::pair<int,int> size = util::StrToPair(config.getSection("model_image").getProperty("size"));
		oldX = size.first;
		oldY = size.second;
	}

	std::vector<InterestingObject> split(const cv::Mat& image) {  
		std::vector<InterestingObject> result;
		for (Config::Section section: config["input_field"]) {
			InterestingObject piece;
			piece.x = getRealX(util::StrToInt(section.getProperty("x")), image.size());
			piece.y = getRealY(util::StrToInt(section.getProperty("y")), image.size());
			piece.width = getRealX(util::StrToInt(section.getProperty("width")), image.size());
			piece.height = getRealY(util::StrToInt(section.getProperty("height")), image.size());
			piece.image = image.rowRange(piece.y, piece.y + piece.height).colRange(piece.x, piece.x + piece.width).clone();
			result.push_back(piece);
		}
		return result;
	}
};



#endif
