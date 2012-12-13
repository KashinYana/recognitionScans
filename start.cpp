#include "thresholding/thresholding.hpp"
#include "findRectangles/findRectangles.h"
#include "config/config.hpp"
#include "images/image.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>

void writeRectanglesInConfig(const std::vector<cv::Rect_<int> >& rectangles, Config& config, const std::string& nameConfigResult)
{
    for(int i = 0; i < rectangles.size(); i++)
    {
        Config::Section section;
        section.add("x", util::IntToStr(rectangles[i].x));
        section.add("y", util::IntToStr(rectangles[i].y));
        section.add("width", util::IntToStr(rectangles[i].width));
        section.add("height", util::IntToStr(rectangles[i].height));
        config.add("rectangle", section);
    }
    config.write(nameConfigResult);
}

std::vector<cv::Rect_<int> > findRectangles(const std::string& fileName, int red, int green, int blue, std::string& namePaintPicture)
{
    cv::Mat matrix = Image::read(fileName);
    FindRectangles find(matrix, red, green, blue);
	Image::show(matrix);
    std::vector<cv::Rect_<int> > rectangles = find.find();
    DrawRectangles draw(matrix);
    draw.draw(rectangles).copyTo(matrix);
    imwrite(namePaintPicture, matrix);
    return rectangles;
}

int main()
{
    std::string nameMainConfig = "config.cfg";
    std::string nameConfigResult = "config_marks.cfg";
    std::string namePaintPicture = "format.jpg";
    std::string nameRepaintingFile = "repaiting";

    Config config = Config::parse("config.cfg");

    std::string anchors;
    int numberAnchors;

    for (Config::Section section: config["anchor"]) {
        assert(section["name_file"].size() == 1);
        for (std::string property: section["name_file"]) {
			anchors = property;
		}
        assert(section["count"].size() == 1);
        for (std::string property: section["count"]) {
			 numberAnchors = util::StrToInt(property);
		}
    }
    std::string original;
    for (Config::Section section: config["original"]) {
        assert(section["name_file"].size() == 1);
        for (std::string property: section["name_file"]) {
			original = property;
		}
    }
    int red, green, blue;
    for (Config::Section section: config["color_rectangles"]) {
        assert(section["red"].size() == 1);
        for (std::string property: section["red"]) {
			red = util::StrToInt(property);
		}
		assert(section["green"].size() == 1);
		for (std::string property: section["green"]) {
			green = util::StrToInt(property);
		}
		assert(section["blue"].size() == 1);
		for (std::string property: section["blue"]) {
			blue = util::StrToInt(property);
		}
    }

    std::vector<cv::Rect_<int> > rectangles = findRectangles(original, red, green, blue, namePaintPicture);
    Config configDataRectangles;
    writeRectanglesInConfig(rectangles, configDataRectangles, nameConfigResult);

    IplImage* image = cvLoadImage(namePaintPicture.c_str());
    cvNamedWindow(namePaintPicture.c_str());
    cvShowImage(namePaintPicture.c_str(), image);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvDestroyAllWindows();

    std::cout << "File \"" + namePaintPicture + " \" contains rectangles, that we found. You can change results in \"" + nameConfigResult + "\"." << std::endl;
    std::cout << "If you change results, don't forget execute \"" + nameRepaintingFile + "\"." << std::endl;

    return 0;
}
