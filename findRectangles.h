#ifndef FINDRECTANGLES_H_INCLUDED
#define FINDRECTANGLES_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <utility>
#include <vector>

class FindRectangles
{
public:
    FindRectangles(cv::Mat matrix, int red_, int green_, int blue_) :
     epsilon(20), deviationX(10), deviationY(10), maxSize(300), minValueColor(0),
     maxValueColor(255)
    {
        red = red_;
        blue = blue_;
        green = green_;
        matrix.copyTo(picture);
        matrix.copyTo(mark);
        for(int i = 0; i < mark.rows; i++)
        {
            for(int j = 0; j < mark.cols; j++)
            {
                for(int numberColor = 0; numberColor < 3; numberColor++)
                    mark.at<cv::Vec3b>(i, j)[numberColor] = minValueColor;
            }
        }
    }
    std::vector<cv::Rect_<int> > find()
    {
        std::vector<cv::Rect_<int> > answer;
        for(int i = 0; i < picture.rows; i++)
        {
            for(int j = 0; j < picture.cols; j++)
            {
                if(goodColor(i, j) && free(i, j))
                {
                    std::pair<int, int> rightDownAngle(i, j);
                    std::pair<int, int> leftUpAngle(i, j);
                    dfs(i, j, leftUpAngle, rightDownAngle);
                    answer.push_back(cv::Rect_<int>(leftUpAngle.second, leftUpAngle.first,
                                                     rightDownAngle.second - leftUpAngle.second, rightDownAngle.first - leftUpAngle.first));
                }
            }
        }
        return answer;
    }

    cv::Mat wrireNumbers(const std::vector<cv::Rect_<int> >& rectangles, cv::Scalar colorRectangles = CV_RGB(255, 0, 0),
                         cv::Scalar colorText = CV_RGB(255, 0, 0))
    {
        cv::Mat answer;
        picture.copyTo(answer);
        for(int i = 0; i < rectangles.size(); i++)
        {
           cv::rectangle(answer, rectangles[i], colorRectangles, 2);
           char name[maxSize];
           sprintf(name, "%d", i);
           cv::putText(answer, std::string(name), cvPoint(rectangles[i].x, rectangles[i].y + rectangles[i].height/2),
                        cv::FONT_HERSHEY_COMPLEX, 1, colorText, 2);
        }
        return answer;
    }

private:

    const int maxValueColor;
    const int minValueColor;
    const int maxSize;
    const int deviationX, deviationY;
    const int epsilon;
    cv::Mat picture;
    cv::Mat mark;
    int red, green, blue;

    void dfs(int i, int j, std::pair<int, int>& leftUpAngle, std::pair<int, int>& ringhtDownAngle)
    {
        if(i + j > ringhtDownAngle.first + ringhtDownAngle.second)
        {
            ringhtDownAngle = std::make_pair(i, j);
        }
        if(i + j < leftUpAngle.first + leftUpAngle.second)
        {
            leftUpAngle = std::make_pair(i, j);
        }
        for(int numberColor = 0; numberColor < 3; numberColor++)
        {
            mark.at<cv::Vec3b>(i, j)[numberColor] = maxValueColor;
        }
        for(int deltaX = -deviationX; deltaX <= deviationX; deltaX++)
        {
            for(int deltaY = -deviationY; deltaY <= deviationY; deltaY++)
            {
                if(inTable(i + deltaX, j + deltaY) && goodColor(i + deltaX, j + deltaY) && free(i + deltaX, j + deltaY))
                {
                    dfs(i + deltaX, j + deltaY, leftUpAngle, ringhtDownAngle);
                }
            }
        }
    }

    bool inTable(int i, int j)
    {
        return i >= 0 && i < picture.rows && j >= 0 && j < picture.cols;
    }

    bool goodColor(int i, int j)
    {
        return abs(picture.at<cv::Vec3b>(i, j)[0] - blue) < epsilon && abs(picture.at<cv::Vec3b>(i, j)[1] - green) < epsilon
         && abs(picture.at<cv::Vec3b>(i, j)[2] - red) < epsilon;
    }

    bool free(int i, int j)
    {
        return mark.at<cv::Vec3b>(i, j)[0] == minValueColor;
    }
};

#endif // FINDRECTANGLES_H_INCLUDED
