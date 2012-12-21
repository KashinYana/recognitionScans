#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "../../images/image.hpp"
#include "../../thresholding/thresholding.hpp"
#include <iostream>

class ImageProcessor {

private:
	bool valid(int i, int j) {
		return (0 <= i && i < image.rows && 0 <= j && j < image.cols);
	}

	void dfs(int i, int j, int c) {
		if (!valid(i, j)) 
			return;
		if (used[i][j] != 0) {
			return;
		}
		if (image.at<uchar>(i,j) != BLACK) {
			return;
		}
		used[i][j] = c;
		dfs(i - 1, j, c);
		dfs(i + 1, j, c);
		dfs(i, j - 1, c);
		dfs(i, j + 1, c);
	}

	cv::Mat image;
	std::vector < std::vector <int> > used;

public:

	 ImageProcessor(cv::Mat& img): image(img) {
		 used.resize(image.rows);
		 for (int i = 0; i < used.size(); i ++) {
			 used[i].resize(image.cols);
		 }
	 }

	 static const int WHITE = 255;
	 static const int BLACK = 0;

	 bool processRow(int row) {
		int cnt = 0;
		for (int j = 0; j < image.cols; j ++) {
			if (image.at<uchar>(row, j) == BLACK) {
				cnt ++;
			}
		}
		if (cnt > 0.6 * image.cols) {
			for (int j = 0; j < image.cols; j ++) {
				image.at<uchar>(row, j) = (uchar)WHITE;
			}
			return true;
		} else {
			return false;
		}		
	}

	 bool processCol(int col) {
		int cnt = 0;
		for (int i = 0; i < image.rows; i ++) {
			if (image.at<uchar>(i, col) == BLACK) {
				cnt ++;
			}
		}
		if (cnt > 0.6 * image.rows) {
			for (int i = 0; i < image.cols; i ++) {
				image.at<uchar>(i, col) = (uchar)WHITE;
			}
			return true;
		} else {
			return false;
		}		
	}

	cv::Mat process(int newSize) {
		image = Thresholding(image).transform();
		
		// cut egdes:

		for (int i = 0; i < image.rows; i ++) {
			if (!processRow(i)) {
				break;
			}
		}

		for (int i = image.rows - 1; i >= 0; i --) {
			if (!processRow(i)) {
				break;
			}
		
		}

		for (int i = 0; i < image.cols; i ++) {
			if (!processCol(i)) {
				break;
			}
		}

		for (int i = image.cols - 1; i >= 0; i --) {
			if (!processCol(i)) {
				break;
			}
		}

		for (int i = 0; i < used.size(); i ++)
			for (int j = 0; j < used[i].size(); j ++)
				used[i][j] = 0;

		int L = 1;
		for (int i = 0; i < image.rows; i ++)
			for (int j = 0; j < image.cols; j ++) 
				if (image.at<uchar>(i, j) == BLACK && used[i][j] == 0) {
					dfs(i, j, L);
					L ++;
				}

		std::map<int,int> cnt;
		for (int i = 0; i < image.rows; i ++)
			for (int j = 0; j < image.cols; j ++) 
				if (used[i][j] != 0)
					cnt[used[i][j]] ++;

		int max_value = 0;
		int best_comp;
		for (std::map<int,int>::iterator it = cnt.begin(); it != cnt.end(); it ++)
			if (it->second > max_value) {
				max_value = it->second;
				best_comp = it->first;
			}
		
		for (int i = 0; i < image.rows; i ++)
			for (int j = 0; j < image.cols; j ++) 
				if (used[i][j] != best_comp) {
					image.at<uchar>(i,j) = WHITE;
				}

		int minRow = image.rows;
		int maxRow = -1;
		int minCol = image.cols;
		int maxCol = -1;
		
		for (int i = 0; i < image.rows; i ++)
			for (int j = 0; j < image.cols; j ++)
				if (image.at<uchar>(i,j) == BLACK) {
					minRow = std::min(minRow, i);
					maxRow = std::max(maxRow, i);
					minCol = std::min(minCol, j);
					maxCol = std::max(maxCol, j);
				}
		int X = minCol;
		int Y = minRow;
		int W = maxCol - minCol + 1;
		int H = maxRow - minRow + 1;
		int X0 = X - std::max(0, H - W) / 2;
		int Y0 = Y - std::max(0, W - H) / 2;
		try {
			image = Image::getSubMatrix(image, X0, Y0, std::max(W, H), std::max(W,H));
		} catch (...) {
		}
		cv::Mat newImage;
		cv::resize(image, newImage, cv::Size(newSize, newSize));
		newImage = Thresholding(newImage).transform();
		return newImage;
	}
};

#endif 
