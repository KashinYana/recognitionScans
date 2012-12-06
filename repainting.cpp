#include "tresholding/thresholding.h"
#include "findRectangles/findRectangles.h"
#include "config/config.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>

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

    std::vector<cv::Rect_<int> > rectangles;

    for (Config::Section section: configData["rectangle"]) {
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

    IplImage* image = cvLoadImage(original.c_str());
    cv::Mat matrix(image);
    DrawRectangles draw(matrix);
    draw.draw(rectangles).copyTo(matrix);
    imwrite(namePaintPicture, matrix);

    cvNamedWindow(namePaintPicture.c_str());
    IplImage imageNewResult((IplImage)matrix);
    cvShowImage(namePaintPicture.c_str(), &imageNewResult);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvDestroyAllWindows();

    std::cout << "File \"" + namePaintPicture + " \" contains new version dates of rectangles. You can change results in \"" + nameConfigResult + "\"." << std::endl;
    std::cout << "If you change results, don't forget execute \"" + nameRepaintingFile + "\"." << std::endl;

    return 0;
}

