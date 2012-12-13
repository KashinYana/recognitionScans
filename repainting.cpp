#include "tresholding/thresholding.h"
#include "findRectangles/findRectangles.h"
#include "config/config.hpp"
#include "images/image.hpp"


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>

std::vector<cv::Rect_<int> > readRectangles(Config& config)
{
    std::vector<cv::Rect_<int> >rectangles;
    for (Config::Section section: config["rectangle"]) {
        int x, y, width, height;
        for (std::string property: section["x"]) {
			x = util::StrToInt(property);
		}
        for (std::string property: section["y"]) {
			y = util::StrToInt(property);
		}
		for (std::string property: section["width"]) {
			width = util::StrToInt(property);
		}
		for (std::string property: section["height"]) {
			height = util::StrToInt(property);
		}
		rectangles.push_back(cv::Rect_<int>(x, y, width, height));
    }
    return rectangles;
}

std::vector<cv::Point> readPoints(Config& config)
{
    std::vector<cv::Point > points;
    for (Config::Section section: config["anchor"]) {
        for (std::string property: section["position"]) {
			int positionComma = property.find(",");
			int x = util::StrToInt(property.substr(1, positionComma - 1));
			int y = util::StrToInt(property.substr(positionComma + 1, property.length() - 1 - positionComma - 1));
			points.push_back(cv::Point(x, y));
		}
    }
    return points;
}

void drawRectangles(const std::vector<cv::Rect_<int> >& rectangles, cv::Mat picture)
{
    DrawRectangles draw(picture);
    draw.draw(rectangles).copyTo(picture);
}

void drawPoints(const std::vector<cv::Point>& points, cv::Mat picture)
{
    DrawPoints draw(picture);
    draw.draw(points).copyTo(picture);
}

int main()
{

    std::string nameMainConfig = "config.cfg";
    std::string nameConfigResult = "config_marks.cfg";
    std::string namePaintPicture = "format.jpg";
    std::string nameRepaintingFile = "repaiting";

    Config configData = Config::parse(nameConfigResult);
    Config config = Config::parse(nameMainConfig);

    std::string original;
    for (Config::Section section: config["original"]) {
        assert(section["name_file"].size() == 1);
        for (std::string property: section["name_file"]) {
			original = property;
		}
    }
    cv::Mat origalPicture = Image::read(original);
    cv::Mat repaintPicture;
    origalPicture.copyTo(repaintPicture);

    std::vector<cv::Rect_<int> > rectangles = readRectangles(configData);
    drawRectangles(rectangles, repaintPicture);
    std::vector<cv::Point> points = readPoints(configData);
    drawPoints(points, repaintPicture);

    imwrite(namePaintPicture, repaintPicture);

    cvNamedWindow(namePaintPicture.c_str());
    IplImage imageNewResult((IplImage)repaintPicture);
    cvShowImage(namePaintPicture.c_str(), &imageNewResult);
    cvWaitKey(0);
    cvDestroyAllWindows();

    std::cout << "File \"" + namePaintPicture + " \" contains new version dates of rectangles. You can change results in \"" + nameConfigResult + "\"." << std::endl;
    std::cout << "If you change results, don't forget execute \"" + nameRepaintingFile + "\"." << std::endl;
    return 0;
}

