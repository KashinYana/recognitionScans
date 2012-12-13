#ifndef SPLITTER_H_
#define SPLITTER_H_

#include "../config/config.hpp"
#include "../images/image.hpp"
#include "../util/util.hpp"

class Splitter {
  private:
	Config config;
  public:
	Splitter(Config cfg): config(cfg) {
	}

	std::vector<cv::Mat> split(const cv::Mat& image) {  
		std::vector<cv::Mat> result;
		for (Config::Section section: config["input_field"]) {
			int x = util::StrToInt(section.getProperty("x"));
			int y = util::StrToInt(section.getProperty("y"));
			int width = util::StrToInt(section.getProperty("width"));
			int heigth = util::StrToInt(section.getProperty("heigth"));
			cv::Mat current = image.rowRange(y, y + heigth).colRange(x, x + width).clone();
			result.push_back(current);
		}
		return result;
	}
};



#endif
