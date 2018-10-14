#include <iostream>
#include <sstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"

#include "LLWindow.h"
#include "cv_algorithm.h"

using namespace std;
using namespace cv;

int main()
{
	{
		for (int i = 0; i< 13; i++) 
		{
			stringstream iss, oss;
			iss << "loading-" << i << ".png";
			oss << "loading--" << i << ".png";
			cutout_image(iss.str(), oss.str());
		}
		
		return 0;
	}
	auto image = imread("./tool.jpg");

	//for (int i = 0; i < image.rows; i++) 
	//{
	//	for (int j = 0; j < image.cols; j++)
	//	{
	//		image.at<cv::Vec3b>(i, j)[0] = 0;
	//	}
	//}
	//cv::imwrite("tool.bmp", image);
	//return 0;

	Mat gray_image;
	rgb2gray(image, gray_image);

	show_image(gray_image);

	//Mat bImage = get_binary(gray_image);

	fuliye(gray_image);

	//Mat hist_image = get_hist(gray_image);
	
	//Mat bImage = get_binary(gray_image);

	//Mat element(5, 5, CV_8U, cv::Scalar(1));
	Mat result_image;
	//erode(bImage, result_image, Mat());
	//erode(result_image, result_image, Mat());
	//dilate(result_image, result_image, Mat());
	//dilate(result_image, result_image, Mat());
	//dilate(result_image, result_image, Mat());


	//result_image = ~result_image;

	system("pause");
}