#ifndef __HANDWRITTEN_DIGITS_RECOGNIZER_H__
#define __HANDWRITTEN_DIGITS_RECOGNIZER_H__

#include <vector>
#include <iostream>
#include <time.h>

#include "../../images/image.hpp"
#include "../../util/util.hpp"
#include "../../thresholding/thresholding.hpp"
#include "processor.hpp"

class HandwrittenDigitsRecognizer {
private:
	static std::vector<cv::Mat> train_input;
	static std::vector<int> train_answer;
	static CvKNearest nearest;

	static const int IMAGE_SIZE = 28;

	static void loadData(int value) {
		std::cerr << "loadData " << value << std::endl;
		std::string file = "../recognition/handwritten_digits/train_data/data" + util::IntToStr(value);
		std::cerr << file << std::endl;
		std::ifstream fin(file);
		while (!fin.eof() && !fin.fail()) {
			cv::Mat image(IMAGE_SIZE, IMAGE_SIZE, CV_8UC1);
			bool fail = false;
			for (int i = 0; i < IMAGE_SIZE; i ++) {
				for (int j = 0; j < IMAGE_SIZE; j ++) {
					unsigned int c = (uchar) fin.get();					
					if (c == -1) {
						fail = true;
					}
					image.at<uchar>(i,j) = (uchar) 255 - c;					
				}
			}
			if (!fail) {
				train_input.push_back(ImageProcessor(image).process(IMAGE_SIZE));
				train_answer.push_back(value);
			}
		}
	}

	static void train() {
		int size = IMAGE_SIZE * IMAGE_SIZE;
		cv::Mat input(train_input.size(), size, CV_32FC1);
		cv::Mat output(train_input.size(), 1, CV_32FC1);
		for (int k = 0; k < train_input.size(); k ++) {
			for (int i = 0; i < IMAGE_SIZE; i ++)
				for (int j = 0; j < IMAGE_SIZE; j ++)
					input.at<float>(k, i * IMAGE_SIZE + j) = train_input[k].at<uchar>(i, j) / (float) 255;
			output.at<float>(k, 0) = (uchar) train_answer[k] / (float) 10;
		}
		nearest.train(input, output);
	}
		
	static void learn() {
		for (int i = 0; i < 10; i ++) {
			loadData(i);
		}
		int t1 = clock();
		train();
		int t2 = clock();
		std::cerr << "Learned in " << (t2 - t1) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
	}

	int sqr(int x) {
		return x * x;
	}
		
public:
	static bool learned;

	HandwrittenDigitsRecognizer() {
		if (!learned) {
			learn();
			learned = true;
		}
	}

	std::string recognize(cv::Mat image) {
		cv::Mat resizedImage;
		cv::resize(ImageProcessor(image).process(IMAGE_SIZE), resizedImage, cv::Size(IMAGE_SIZE, IMAGE_SIZE));
		Image::Image newImage = Thresholding(resizedImage).transform();
		cv::Mat valueToFind(1, IMAGE_SIZE * IMAGE_SIZE, CV_32FC1);
		for (int i = 0; i < IMAGE_SIZE; i ++)
			for (int j = 0; j < IMAGE_SIZE; j ++)
				valueToFind.at<float>(0, i * IMAGE_SIZE + j) = newImage.at<uchar>(i, j) / (float) 255;
		cv::Mat result;
		cv::Mat neighborResponses;
		cv::Mat dists;
		nearest.find_nearest(valueToFind, nearest.get_max_k(), result, neighborResponses, dists);
		return util::IntToStr(round(result.at<float>(0,0) * 10));
		int best = (int) 1e9;
		int answer = 0;
		cv::Mat bestImage;
		std::vector < std:: pair <int, int> > v;
		for (int k = 0; k < train_input.size(); k ++) {
			int cur = 0; 
			for (int i = 0; i < IMAGE_SIZE; i ++) {
				for (int j = 0; j < IMAGE_SIZE; j ++) {
					cur += sqr((int)train_input[k].at<uchar>(i,j) - (int)newImage.at<uchar>(i,j));
				}
			}
			v.push_back(std::make_pair(cur, train_answer[k]));
			if (cur < best) {
				best = cur;
				answer = train_answer[k];
				bestImage = train_input[k];
			}
		}
		sort(v.begin(), v.end());
		const int K = 30;
		std::map<int,int> cnt;
		for (int i = 0; i < K; i ++)
			cnt[v[i].second] ++;
		int maxv = 0;
		for (std::map<int,int>::iterator it = cnt.begin(); it != cnt.end(); it ++) {
			if (it->second > maxv) {
				maxv = it->second;
				answer = it->first;
			}
		}
		return util::IntToStr(answer);
	}
};
bool HandwrittenDigitsRecognizer::learned = false;
std::vector<cv::Mat> HandwrittenDigitsRecognizer::train_input;
std::vector<int> HandwrittenDigitsRecognizer::train_answer;
CvKNearest HandwrittenDigitsRecognizer::nearest;

#endif
