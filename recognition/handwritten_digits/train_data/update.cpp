#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

#include "../../../images/image.hpp"
#include "../../../util/util.hpp"

const int IMAGE_SIZE = 28;

const int ESC_KEY = 27;

int image_count = 0;

void loadData(int value) {
	std::cerr << "loadData " << value << std::endl;
	std::string file = "data" + util::IntToStr(value);
	std::string file_out = "data" + util::IntToStr(value) + ".new";
	std::ifstream fin(file);
	std::ofstream fout(file_out);
	while (!fin.eof() && !fin.fail()) {
		cv::Mat image(IMAGE_SIZE, IMAGE_SIZE, CV_8UC1);
		for (int i = 0; i < IMAGE_SIZE; i ++) {
			for (int j = 0; j < IMAGE_SIZE; j ++) {
				image.at<uchar>(i,j) = (uchar) 255 - (uchar)fin.get();
			}
		}
		int key = Image::show(image);
		if (key != ESC_KEY) {
			for (int i = 0; i < IMAGE_SIZE; i ++)
				for (int j = 0; j < IMAGE_SIZE; j ++) {
					uchar value = (uchar) 255 - image.at<uchar>(i, j);
					fout.put(value);
				}				
		}
		std::cerr << ++ image_count << std::endl;
	}
	fout.close();
}

int main() {
	for (int i = 0; i < 10; i ++) {
		loadData(i);
	}
}
