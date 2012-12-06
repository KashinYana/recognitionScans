#include "recognize_number.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
    for(int number = 0; number <= 9; number++)
    {
        char filename[100];
        sprintf(filename, "number%d.jpg", number);
        IplImage* image = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
        assert(image != NULL);
        cv::Mat mtx(image);
        if(number != recongizeNumber::recongize(mtx))
        {
            std::cerr << "answer"<< number <<" : " << recongizeNumber::recongize(mtx) << std::endl;
            exit(1);
        }
        cvReleaseImage(&image);
    }

    IplImage* image = cvLoadImage("Image0.jpg");
    assert(image != NULL);
    cv::Mat mtx(image);
    recongizeNumber::ClearTrash clearTrash(mtx, 50);
    cv::Mat newMtx(clearTrash.clear());
    IplImage newPicture((IplImage)newMtx);
    cvNamedWindow("image");
    cvShowImage("image", &newPicture);

    cvWaitKey(0);
    cvReleaseImage(&image);
    cvDestroyAllWindows();

    return 0;
}
