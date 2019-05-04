#include<opencv2/opencv.hpp>
#include<iostream>
#include <ctime>

using namespace std;
using namespace cv;

inline void Easy_bin(float left_pivot, float right_pivot, const Mat src1, Mat &img) {
	img = src1.clone();
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++){
				if ((img.at<uchar>(i, j) > left_pivot) &&
				(img.at<uchar>(i, j) < right_pivot)){
					img.at<uchar>(i, j) = 255;
				}
				else {
					img.at<uchar>(i, j) = 0;
				}
			
		}
	}

}

void Bradley_threshold(const Mat src, Mat &res, int width, int height) {
	res = src.clone();
	const int S = width / 10;
	int s2 = S / 2;
	const float t = 0.15;
	unsigned long* integral_image = 0;
	long sum = 0;
	int count = 0;
	int index;
	int x1, y1, x2, y2;

	//Интегральное изображение 
	integral_image = new unsigned long[width*height * sizeof(unsigned long*)];

	for (int i = 0; i < width; i++) {
		sum = 0;
		for (int j = 0; j < height; j++) {
			index = j * width + i;
			sum += src.at<uchar>(i,j);
			if (i == 0)
				integral_image[index] = sum;
			else
				integral_image[index] = integral_image[index - 1] + sum;
		}
	}

	//Границы локальных областей
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			index = j * width + i;

			x1 = i - s2;
			x2 = i + s2;
			y1 = j - s2;
			y2 = j + s2;

			if (x1 < 0)
				x1 = 0;
			if (x2 >= width)
				x2 = width - 1;
			if (y1 < 0)
				y1 = 0;
			if (y2 >= height)
				y2 = height - 1;

			count = (x2 - x1)*(y2 - y1);

			sum = integral_image[y2*width + x2] - integral_image[y1*width + x2] -
				integral_image[y2*width + x1] + integral_image[y1*width + x1];
			if ((long)(src.at<uchar>(i,j) * count) < (long)(sum*(1.0 - t)))
				res.at<uchar>(i, j) = 0;
			else
				res.at<uchar>(i, j) = 255;
		}
	}

	delete[] integral_image;
}



int main(int argc, char *argv[])
{
	bool benchmark = false;
	int iter = 4;
	if (argc < 2)// если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
	{
		cout << "Using <path_image_to_bin> <path_bin_image_save> if need <iter_number_benchmark>" << endl;
		return 0;
	}
	if (argc == 4) 
	{
		benchmark = true;
		iter = atoi(argv[3]);
	}
	string input_img = argv[1];
	string output_img = argv[2];

	Mat img = imread(input_img, 0);
	
	Mat gray;
	Mat gray1;
	
	if (benchmark) {
		unsigned int start_time = clock(); // начальное время
		for (int i = 0; i < iter; ++i) {
			Bradley_threshold(img, gray1, img.rows, img.cols);
		}
		unsigned int end_time = clock(); // конечное время
		unsigned int search_time = end_time - start_time;
		float img_size = img.cols*img.rows;
		cout << "Bradley time: " << (float)(1000000*search_time/(iter*img_size)) << " sec/Mpx" << endl;
		return 0;
	}
	namedWindow("image", WINDOW_NORMAL);
	Easy_bin(20, 150, img, gray);
	Bradley_threshold(img, gray1, img.rows, img.cols);
	
	imshow("image",  gray1);
	imwrite(output_img + "_Easy.png", gray);
	imwrite(output_img + "_Bradley.png", gray1);
	waitKey(0);
	
	return 0;
}