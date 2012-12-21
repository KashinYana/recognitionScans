#ifndef THRESHOLDING_H_INCLUDED
#define THRESHOLDING_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include "../images/image.hpp"

class Thresholding
{
private:
    cv::Mat matrix;
    const int middleValue;
    const int maxValue;
    const int minValue;
    const int upperBound;
    const int downBound;
    int numberColors;
    int shift;
    std::vector<std::vector<std::vector<int> > > partialSum;
public:
    explicit Thresholding(const cv::Mat& matrix_, int shift_ = 11)
    : matrix(matrix_), middleValue(128), minValue(0), maxValue(255),
    upperBound(245), downBound(15), numberColors(3)
    {
//		matrix = Image::makeBlackAndWhite(matrix_);
//		cvtColor(matrix_, matrix, CV_RGB2GRAY);
		if (matrix.type() == CV_8UC1) {
			numberColors = 1;
		} else if (matrix.type() == CV_8UC3) {
		} else {
			throw std::logic_error("unsupported matrix type");
		}
        shift = shift_;
        resizePartialSum();
        initPartialSum();
    }

    cv::Mat simpleTransform(int middleValue_ = 128)
    {
        cv::Mat result(matrix.rows, matrix.cols, CV_8UC1);
        for (int i = 0; i < result.rows; i++)
        {
            for (int j = 0; j < result.cols; j++)
            {
                for(int k = 0; k < numberColors; k++)
                {
                    if(matrix.at<cv::Vec3b>(i, j)[k] > middleValue_)
                    {
                        result.at<uchar>(i, j) = maxValue;
                    }
                    else
                    {
                        result.at<uchar>(i, j) = minValue;
                    }
                }
            }
        }
        return result;
    }

    cv::Mat transform(int upperBound_ = 245, int downBound_ = 15)
    {
        cv::Mat result(matrix.rows, matrix.cols, CV_8UC1);
		if (matrix.type() == CV_8UC3) { // process normal image 
            for (int i = 0; i < result.rows; i++)
            {
                for (int j = 0; j < result.cols; j++)
                {
                    for(int k = 0; k < numberColors; k++)
                    {
                        int leftx = i;
                        int rightx = i + 2*shift;
                        int lefty = j;
                        int righty = j + 2*shift;
                        int threshold = partialSum[rightx][righty][k] - partialSum[rightx][lefty][k]
                                - partialSum[leftx][righty][k] + partialSum[leftx][lefty][k];
                        threshold /= (righty - lefty) * (rightx - leftx );
                        if (matrix.at<cv::Vec3b>(i, j)[k] > upperBound_)
                        {
                            result.at<uchar>(i, j) = maxValue;
                        }
                        else if (matrix.at<cv::Vec3b>(i, j)[k] < downBound_)
                        {
                            result.at<uchar>(i, j) = minValue;
                        }
                        else if (matrix.at<cv::Vec3b>(i, j)[k] > threshold)
                        {
                            result.at<uchar>(i, j) = maxValue;
                        }
                        else if (matrix.at<cv::Vec3b>(i, j)[k] < threshold)
                        {
                            result.at<uchar>(i, j) = minValue;
                        }
                        else if (matrix.at<cv::Vec3b>(i, j)[k] < middleValue)
                        {
                            result.at<uchar>(i, j) = minValue;
                        }
                        else
                        {
                            result.at<uchar>(i, j) = maxValue;
                        }
                    }
                }
            }
            return result;
		}
		if (matrix.type() == CV_8UC1) { // process white-black image
            for (int i = 0; i < result.rows; i++)
            {
                for (int j = 0; j < result.cols; j++)
                {
                    int leftx = i;
                    int rightx = i + 2*shift;
                    int lefty = j;
                    int righty = j + 2*shift;
                    int threshold = partialSum[rightx][righty][0] - partialSum[rightx][lefty][0]
                            - partialSum[leftx][righty][0] + partialSum[leftx][lefty][0];
                    threshold /= (righty - lefty) * (rightx - leftx );
                    if (matrix.at<uchar>(i, j) > upperBound_)
                    {
                        result.at<uchar>(i, j) = maxValue;
                    }
                    else if (matrix.at<uchar>(i, j) < downBound_)
                    {
                        result.at<uchar>(i, j) = minValue;
                    }
                    else if (matrix.at<uchar>(i, j) > threshold)
                    {
                        result.at<uchar>(i, j) = maxValue;
                    }
                    else if (matrix.at<uchar>(i, j) < threshold)
                    {
                        result.at<uchar>(i, j) = minValue;
                    }
                    else if (matrix.at<uchar>(i, j) < middleValue)
                    {
                        result.at<uchar>(i, j) = minValue;
                    }
                    else
                    {
                        result.at<uchar>(i, j) = maxValue;
                    }
                }
            }
            return result;
		}
		throw std::logic_error("unsupported matrix type for thresholding");
    }

private:

    void resizePartialSum()
    {
        partialSum.resize(matrix.rows + 2*shift);
        for (int i = 0; i < partialSum.size(); i++)
        {
            partialSum[i].resize(matrix.cols  + 2*shift);
            for(int j = 0; j < partialSum[i].size(); j++)
            {
                partialSum[i][j].resize(numberColors);
            }
        }
    }

    void initPartialSum()
    {
        for (int i = 0; i < partialSum.size(); i++)
        {
            for (int j = 0; j < partialSum[i].size(); j++)
            {
                for(int k = 0; k < numberColors; k++)
                {
                    if(i < shift || j < shift || i >= matrix.rows + shift || j >= matrix.cols + shift)
                    {
                        partialSum[i][j][k] = middleValue;
                    }
                    else
                    {
                        partialSum[i][j][k] = matrix.at<cv::Vec3b>(i - shift, j - shift)[k];
                    }
                    if(i > 0)
                    {
                        partialSum[i][j][k] += partialSum[i - 1][j][k];
                    }
                    if(j > 0)
                    {
                        partialSum[i][j][k] += partialSum[i][j - 1][k];
                    }
                    if(i > 0 && j > 0)
                    {
                        partialSum[i][j][k] -= partialSum[i - 1][j - 1][k];
                    }
                }
            }
        }
    }
};

#endif // THRESHOLDING_H_INCLUDED
