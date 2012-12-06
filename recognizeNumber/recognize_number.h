#ifndef RECOGNIZE_NUMBER_H_INCLUDED
#define RECOGNIZE_NUMBER_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../findRectangles/findRectangles.h"

namespace recongizeNumber{

class ClearTrash {
public:
    explicit ClearTrash(const cv::Mat& picture_, int radius_ = 10):
    picture(picture_), radius(radius_), minValue(0), maxValue(255)
    {

    }
    cv::Mat clear()
    {
        cv::Mat answer(picture);
        FindRectangles find(picture, minValue, minValue, minValue); //black
        std::vector<cv::Rect_<int> > rectangles = find.find();
        for(int i = 0; i < rectangles.size(); i++)
        {
            if(rectangles[i].width < radius && rectangles[i].height < radius)
            {
                for(int x = rectangles[i].x; x <= rectangles[i].x + rectangles[i].width; x++)
                {
                    for(int y = rectangles[i].y; y <= rectangles[i].y + rectangles[i].height; y++)
                    {
                        for(int k = 0; k < 3; k++)
                        {
                            answer.at<cv::Vec3b>(y, x)[k] = maxValue;
                        }
                    }
                }
            }
        }
        return answer;
    }
private:
    const cv::Mat& picture;
    int radius;
    const int minValue;
    const int maxValue;
};


class Point
{
public:
    int x, y;
    Point(int x_, int y_)
    {
        x = x_;
        y = y_;
    }
    Point()
    {
        x = y = 0;
    }
};

class Line
{
public:
    Point first, second;
    Line(Point first_, Point second_)
    {
        first = first_;
        second = second_;
    }
};

class Number
{
private:
    std::vector<Line> lines;
    cv::Mat picture;
    std::vector<bool> necessaryLines;
    int minValuePixel, maxValuePixel;
public:
    Number(const cv::Mat& picture_, const std::vector<int>& numberNecessaryLines)
    {
        minValuePixel = 0;
        maxValuePixel = 255;
        picture = picture_;
        add_lines();
        necessaryLines.resize(lines.size());
        mark_necessary_lines(numberNecessaryLines);
        correct_lines(20);
    }
    void add_lines()
    {
        lines.push_back(Line(Point(0, 0), Point(0, picture.cols - 1)));
        lines.push_back(Line(Point(0, picture.cols - 1), Point(picture.rows / 2, picture.cols - 1)));
        lines.push_back(Line(Point(picture.rows / 2, picture.cols - 1), Point(picture.rows - 1, picture.cols - 1)));
        lines.push_back(Line(Point(picture.rows - 1, 0), Point(picture.rows - 1, picture.cols - 1)));
        lines.push_back(Line(Point(picture.rows / 2, 0), Point(picture.rows - 1, 0)));
        lines.push_back(Line(Point(0, 0), Point(picture.rows / 2, 0)));
        lines.push_back(Line(Point(0, picture.cols - 1), Point(picture.rows / 2, 0)));
        lines.push_back(Line(Point(picture.rows / 2, 0), Point(picture.rows / 2, picture.cols - 1)));
        lines.push_back(Line(Point(picture.rows / 2, picture.cols - 1), Point(picture.rows - 1, 0)));
    }
    void add_dabate_angles(std::vector<Point>& debateAngles)
    {
        debateAngles.push_back(Point(0, 0));
        debateAngles.push_back(Point(0, picture.cols - 1));
        debateAngles.push_back(Point(picture.rows / 2, 0));
        debateAngles.push_back(Point(picture.rows / 2, picture.cols - 1));
        debateAngles.push_back(Point(picture.rows - 1, 0));
        debateAngles.push_back(Point(picture.rows - 1, picture.cols - 1));

    }
    void mark_necessary_lines(const std::vector<int>& numberLines)
    {
        for(int i = 0; i < numberLines.size(); i++)
        {
            necessaryLines[numberLines[i]] = true;
        }
    }
    void correct_lines(int delta)
    {
        std::vector<Point> debateAlgles;
        add_dabate_angles(debateAlgles);
        std::vector<int> markDebateAlgles(debateAlgles.size());
        for(int i = 0; i < lines.size(); i++)
        {
            if(necessaryLines[i])
            {
                for(int j = 0; j < debateAlgles.size(); j++)
                {
                    if(cross(debateAlgles[j], delta, lines[i].first) ||
                       cross(debateAlgles[j], delta, lines[i].second))
                    {
                        markDebateAlgles[j] = 1;
                    }
                }
            }
        }
        for(int i = 0; i < lines.size(); i++)
        {
            if(!necessaryLines[i])
            {
                for(int j = 0; j < debateAlgles.size(); j++)
                {
                    if(markDebateAlgles[j] == 1 && cross(debateAlgles[j], delta, lines[i].first))
                    {
                        if(lines[i].first.x == lines[i].second.x)
                            lines[i].first.y += delta;
                        else if(lines[i].first.y == lines[i].second.y)
                            lines[i].first.x += delta;
                        else
                        {
                            lines[i].first.y -= delta;
                            lines[i].first.x += delta;
                        }
                    }
                    if(markDebateAlgles[j] == 1 && cross(debateAlgles[j], delta, lines[i].second))
                    {
                        if(lines[i].first.x == lines[i].second.x)
                            lines[i].second.y -= delta;
                        else if(lines[i].first.y == lines[i].second.y)
                            lines[i].second.x -= delta;
                        else
                        {
                            lines[i].second.x -= delta;
                            lines[i].second.y += delta;
                        }
                    }
                }
            }
        }
    }
    bool cross(const Point& pointCenter, int delta, const Point& point)
    {
        return std::max(abs(point.x - pointCenter.x), abs(point.y - pointCenter.y)) <= delta;
    }
    double similarity()
    {
        double sumGoodPixels = 0;
        double allPixels = 0;
        for(int i = 0; i < lines.size(); i++)
        {
            if(lines[i].first.x == lines[i].second.x)
            {
                for(int y = lines[i].first.y; y < lines[i].second.y; y++)
                {
                    int x = lines[i].first.x;
                    if(isGoodPixel(x, y, i))
                    {
                        sumGoodPixels++;
                    }
                    allPixels++;
                }
            }
            else if(lines[i].first.y == lines[i].second.y)
            {
                for(int x = lines[i].first.x; x < lines[i].second.x; x++)
                {
                    int y = lines[i].first.y;
                    if(isGoodPixel(x, y, i))
                    {
                        sumGoodPixels++;
                    }
                    allPixels++;
                }
            }
            else
            {
                for(int x = lines[i].first.x; x < lines[i].second.x; x++)
                {
                    int lenghtX = abs(lines[i].first.x - lines[i].second.x) / 2;
                    int lenghtY = abs(lines[i].first.y - lines[i].second.y);

                    int y = (double)lenghtY / (double)lenghtX * (double)(lines[i].second.x - x);
                    //if(x == 400) {
                    //    std::cout << "400: x; y = " << y << " " << (int)picture.at<cv::Vec3b>(x, y)[1]<< " bool " << isGoodPixel(x, y, i) << std::endl;
                    //    std::cout << lenghtX << " " << lenghtY << " " << lines[i].second.x - x <<std::endl;
                   // }
                    if(isGoodPixel(x, y, i))
                    {
                        //std::cout << "x = " << x << " y = " << y << std::endl;
                        sumGoodPixels++;
                    }
                    allPixels++;
                }
            }
        }
        return sumGoodPixels / allPixels;
    }
private:
    bool isGoodPixel(int x, int y, int numberLine) const
    {
        return (picture.at<cv::Vec3b>(x, y)[1] == minValuePixel && necessaryLines[numberLine])
                ||(picture.at<cv::Vec3b>(x, y)[1] == maxValuePixel && !necessaryLines[numberLine]);
    }
};

std::vector<int> linesNumber(int number)
{
    if(number == 0)
    {
        return {0, 1, 2, 3, 4, 5};
    }
    else if(number == 1)
    {
        return {1, 2, 6};
    }
    else if(number == 2)
    {
        return {0, 1, 8, 3};
    }
    else if(number == 3)
    {
        return {0, 6, 7, 8};
    }
    else if(number == 4)
    {
        return {5, 7, 1, 2};
    }
    else if(number == 5)
    {
        return {0, 5, 7, 2, 3};
    }
    else if(number == 6)
    {
        return {0, 5, 4, 7, 3, 2};
    }
    else if(number == 7)
    {
        return {0, 3, 4};
    }
    else if(number == 8)
    {
        return {0, 1, 2, 3, 4, 5, 7};
    }
    else if(number == 9)
    {
        return {0, 1, 5, 7, 8};
    }
}

int recongize(const cv::Mat& picture)
{
    std::vector<std::pair<double, int> > result;
    for(int i = 0; i <= 9; i++)
    {
        result.push_back(std::make_pair(Number(picture, linesNumber(i)).similarity(), i));
    }
    std::sort(result.begin(), result.end());
    for(int i = 0; i < result.size(); i++)
    {
        std::cout << " ( " << result[i].second << " , " << result[i].first << " ) "<< std::endl;
    }
    std::cout << std::endl;
    return result.back().second;
}
} //end namespace

#endif // RECOGNIZE_NUMBER_H_INCLUDED
