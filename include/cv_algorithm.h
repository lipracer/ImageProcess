#ifndef _CV_ALGORITHM_H_
#define _CV_ALGORITHM_H_

#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#ifdef __WINDOWS__
#include "LLWindow.h"
#endif


void rgb2gray(Mat& oImage, Mat& nImage);

Mat get_hist(Mat& grayImage);


Mat get_edges(Mat& gray_image);

int fuliye(Mat& input);

//vector<LLWindow*> g_v_window;

int cutout_image(string ifile, string ofile);

void show_image(Mat& mat);

<<<<<<< 5e9b2a65e10be3de1afefa6e023025166f6e4ada
#endif
=======
Mat recognise_num(Mat& mat);

Mat rectify_image(char * img_path);

#endif
>>>>>>> add src code
