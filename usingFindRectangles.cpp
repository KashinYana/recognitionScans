#include "findRectangles.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
    char filename[] = "scan.jpg";
    IplImage* image = cvLoadImage(filename);
    cv::Mat mtx(image);
    cvNamedWindow("image");
    cvShowImage("image", image);

    FindRectangles find(mtx, 0, 0, 255);    // Blue

    std::vector<cv::Rect_<int> > rectangles = find.find();

    find.wrireNumbers(rectangles).copyTo(mtx);

    cvNamedWindow("whiteBlackImage");

    IplImage img ((IplImage)mtx);

    cvShowImage("whiteBlackImage", &img);

    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage(&image);

    // удаляем окна
    cvDestroyAllWindows();

    return 0;
}
