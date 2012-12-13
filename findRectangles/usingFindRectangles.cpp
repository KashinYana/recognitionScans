#include "findRectangles.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
    char filename[] = "test-color.jpg";
    IplImage* image = cvLoadImage(filename);
    cv::Mat mtx(image);
    cvNamedWindow("image");
    cvShowImage("image", image);

    FindRectangles find(mtx, 255, 0, 0);    // Red

    std::vector<cv::Rect_<int> > rectangles = find.find();

    DrawRectangles draw(mtx);
    draw.draw(rectangles).copyTo(mtx);
    cvNamedWindow("whiteBlackImage");

    IplImage img ((IplImage)mtx);

    cvShowImage("whiteBlackImage", &img);

    cvWaitKey(0);
    cvReleaseImage(&image);
    cvDestroyAllWindows();

    return 0;
}
