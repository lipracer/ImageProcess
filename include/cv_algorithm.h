#ifndef _CV_ALGORITHM_H_
#define _CV_ALGORITHM_H_

#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "LLWindow.h"

using namespace std;
using namespace cv;


void rgb2gray(Mat& oImage, Mat& nImage);

Mat get_hist(Mat& grayImage);

Mat get_binary(Mat& grayImage);

Mat get_edges(Mat& gray_image);

int fuliye(Mat& input);

//vector<LLWindow*> g_v_window;

int cutout_image(string ifile, string ofile);

void show_image(Mat& mat);

#endif