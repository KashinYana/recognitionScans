#ifndef THRESHOLDING_H_INCLUDED
#define THRESHOLDING_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

class Thresholding
{
private:
    cv::Mat matrix;
    const int middleValue;
    const int maxValue;
    const int minValue;
    const int upperBound;
    const int downBound;
    const int numberColors;
    int shift;
    std::vector<std::vector<std::vector<int> > > partialSum;
public:
    explicit Thresholding(const cv::Mat& matrix_, int shift_ = 11)
    : matrix(matrix_), middleValue(128), minValue(0), maxValue(255),
    upperBound(245), downBound(15), numberColors(3)
    {
//		cvtColor(matrix_, matrix, CV_RGB2GRAY);
        shift = shift_;
        resizePartialSum();
        initPartialSum();
    }

    cv::Mat simpleTransformation(int middleValue_ = 128)
    {
        cv::Mat result = matrix;
        for (int i = 0; i < result.rows; i++)
        {
            for (int j = 0; j < result.cols; j++)
            {
                for(int k = 0; k < numberColors; k++)
                {
                    if(result.at<cv::Vec3b>(i, j)[k] > middleValue_)
                    {
                        result.at<cv::Vec3b>(i, j)[k] = maxValue;
                    }
                    else
                    {
                        result.at<cv::Vec3b>(i, j)[k] = minValue;
                    }
                }
            }
        }
        return result;
    }

    cv::Mat transformation(int upperBound_ = 245, int downBound_ = 15)
    {
        cv::Mat result = matrix;
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
                    if (result.at<cv::Vec3b>(i, j)[k] > upperBound_)
                    {
                        result.at<cv::Vec3b>(i, j)[k] = maxValue;
                    }
                    else if (result.at<cv::Vec3b>(i, j)[k] < downBound_)
                    {
                        result.at<cv::Vec3b>(i, j)[k] = minValue;
                    }
                    else if (result.at<cv::Vec3b>(i, j)[k] > threshold)
                    {
                        result.at<cv::Vec3b>(i, j)[k] = maxValue;
                    }
                    else if (result.at<cv::Vec3b>(i, j)[k] < threshold)
                    {
                        result.at<cv::Vec3b>(i, j)[k] = minValue;
                    }
                    else if (result.at<cv::Vec3b>(i, j)[k] < middleValue)
                    {
                        result.at<cv::Vec3b>(i, j)[k] = minValue;
                    }
                    else
                    {
                        result.at<cv::Vec3b>(i, j)[k] = maxValue;
                    }
                }
            }
        }
        return result;
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
