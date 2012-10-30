#include "thresholding.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char filename[] = "Image0.jpg";
    IplImage* image = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat mtx(image);

    cvNamedWindow("image");
    cvShowImage("image", image);

    Thresholding hold(mtx);

    cv::Mat whiteBlackImage = hold.transformation(245, 30);
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
