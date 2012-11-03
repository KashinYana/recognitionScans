#include "thresholding.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char filename[] = "scan.jpg";
    IplImage* image = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
    assert(image != NULL);
    cv::Mat mtx(image);

    cvNamedWindow("image");
    cvShowImage("image", image);

    Thresholding hold(mtx, 5);

    cv::Mat whiteBlackImage = hold.transformation(255, 0);

    cvNamedWindow("whiteBlackImage");
    IplImage img ((IplImage)whiteBlackImage);
    cvShowImage("whiteBlackImage", &img);


    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage(&image);

    // удаляем окна
    cvDestroyAllWindows();

    return 0;
}
