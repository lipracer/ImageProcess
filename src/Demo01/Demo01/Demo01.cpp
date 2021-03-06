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
	auto image = imread("./card.jpg");


	Mat gray_image;
	rgb2gray(image, gray_image);
	char * img_path = (char*)"./right.jpg";
	auto result = rectify_image(img_path);
	show_image(result);

	system("pause");
}