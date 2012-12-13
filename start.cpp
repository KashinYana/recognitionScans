#include "tresholding/thresholding.h"
#include "images/image.hpp"
#include "findRectangles/findRectangles.h"
#include "config/config.hpp"
#include "rotate/anchor_finder.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>

void writeRectanglesInConfig(const std::vector<cv::Rect_<int> >& rectangles, Config& config)
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
}

void writePointsInConfig(std::vector<cv::Point>& positions, Config& config)
{
    Config::Section section;
    for(int i = 0; i < positions.size(); i++)
    {
        section.add("position", "(" + util::IntToStr(positions[i].x) + ", " + util::IntToStr(positions[i].y) + ")");
    }
    config.add("anchor", section);
}

void writeSizeInConfig(const cv::Mat& matrix, Config& config)
{
    Config::Section section;
    section.add("size", "(" + util::IntToStr(matrix.cols) + ", " + util::IntToStr(matrix.rows) + ")");
    config.add("model_image", section);
}

std::vector<cv::Rect_<int> > findRectangles(const std::string& fileName, int red, int green, int blue)
{
    cv::Mat matrix = Image::read(fileName);
    FindRectangles find(matrix, red, green, blue);
    std::vector<cv::Rect_<int> > rectangles = find.find();
    return rectangles;
}

std::vector<cv::Point> findPoints(const Config& config, const cv::Mat& image, const cv::Mat& anchor, int anchorCount)
{
    std::vector<cv::Point> positions;
    AnchorFinder anchorFinder(config);
    positions = anchorFinder.find(image, anchor, anchorCount);
    return positions;
}

void drawRectangles(const std::vector<cv::Rect_<int> >& rectangles, cv::Mat& result)
{
    DrawRectangles draw(result);
    draw.draw(rectangles,  CV_RGB(128, 128, 128),  CV_RGB(128, 128, 128)).copyTo(result);  /// подумать о цветах
}

void drawPoints(const std::vector<cv::Point>& points, cv::Mat& result)
{
    DrawPoints draw(result);
    draw.draw(points,  CV_RGB(128, 128, 128),  CV_RGB(128, 128, 128)).copyTo(result);  /// подумать о цветах
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

    cv::Mat matrixOriginal = Image::read(original);
    cv::Mat matrixAnchors = Image::read(anchors);
    cv::Mat paintPicture;
    matrixOriginal.copyTo(paintPicture);
    Config configFindData;

    std::vector<cv::Rect_<int> > rectangles = findRectangles(original, red, green, blue);
    drawRectangles(rectangles, paintPicture);
    writeRectanglesInConfig(rectangles, configFindData);
    std::vector<cv::Point> positions = findPoints(config, matrixOriginal, matrixAnchors, numberAnchors);
    writePointsInConfig(positions, configFindData);

    drawPoints(positions, paintPicture);
    imwrite(namePaintPicture, paintPicture);
    writeSizeInConfig(matrixOriginal, configFindData);
    configFindData.write(nameConfigResult);
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
