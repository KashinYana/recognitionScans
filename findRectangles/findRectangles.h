#ifndef FINDRECTANGLES_H_INCLUDED
#define FINDRECTANGLES_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <utility>
#include <vector>
#include <queue>

class Position{
public:
    int x;
    int y;
    Position(int x_, int y_)
    {
        x = x_;
        y = y_;
    }
};

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
                    bfs(i, j, leftUpAngle, rightDownAngle);
                    answer.push_back(cv::Rect_<int>(leftUpAngle.second, leftUpAngle.first,
                                                     rightDownAngle.second - leftUpAngle.second, rightDownAngle.first - leftUpAngle.first));
                }
            }
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

    void bfs(int i, int j, std::pair<int, int>& leftUpAngle, std::pair<int, int>& ringhtDownAngle)
    {
        std::queue<Position> queue;
        queue.push(Position(i, j));
        into_mark(i, j);
        while(!queue.empty())
        {
            Position current = queue.front();
            queue.pop();
            if(current.x + current.y > ringhtDownAngle.first + ringhtDownAngle.second)
            {
                ringhtDownAngle = std::make_pair(current.x, current.y);
            }
            if(current.x + current.y < leftUpAngle.first + leftUpAngle.second)
            {
                leftUpAngle = std::make_pair(current.x, current.y);
            }
            for(int deltaX = -deviationX; deltaX <= deviationX; deltaX++)
            {
                for(int deltaY = -deviationY; deltaY <= deviationY; deltaY++)
                {
                    int newX = current.x + deltaX;
                    int newY = current.y + deltaY;

                    if(inTable(newX, newY) && goodColor(newX, newY) && free(newX, newY))
                    {
                        into_mark(newX, newY);
                        queue.push(Position(newX, newY));
                    }
                }
            }
        }
    }

    void into_mark(int i, int j)
    {
        for(int numberColor = 0; numberColor < 3; numberColor++)
        {
            mark.at<cv::Vec3b>(i, j)[numberColor] = maxValueColor;
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

class DrawRectangles {
public:
    DrawRectangles(const cv::Mat& matrix)
    {
        matrix.copyTo(picture);
    }

    cv::Mat draw(const std::vector<cv::Rect_<int> >& rectangles, cv::Scalar colorRectangles = CV_RGB(255, 0, 0),
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
    cv::Mat picture;
    static const int maxSize = 200;
};

class DrawPoints {
public:
    DrawPoints(const cv::Mat& matrix)
    {
        matrix.copyTo(picture);
    }

    cv::Mat draw(const std::vector<cv::Point>& points, cv::Scalar colorPoints = CV_RGB(255, 0, 0),
                         cv::Scalar colorText = CV_RGB(255, 0, 0))
    {
        cv::Mat answer;
        picture.copyTo(answer);

        for(int i = 0; i < points.size(); i++)
        {
            cv::circle(answer, points[i], 1, colorPoints, -1);
            char name[maxSize];
            sprintf(name, "%d", i);
            cv::putText(answer, std::string(name), points[i],
                        cv::FONT_HERSHEY_COMPLEX, 1, colorText, 1);
        }
        return answer;
    }
private:
    cv::Mat picture;
    static const int maxSize = 200;
};

#endif // FINDRECTANGLES_H_INCLUDED
