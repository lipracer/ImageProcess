#include <memory>

#include "cv_algorithm.h"

#ifdef __APPLE__
#include "es_context.hpp"
extern ALESContext _esContext;
#endif

void rgb2gray(Mat& oImage, Mat& nImage) 
{
	assert(oImage.depth() != 8);
    cvtColor(oImage, nImage, COLOR_RGBA2GRAY);
}

//mat 没有赋值构造？
Mat get_hist(Mat& grayImage) 
{
	cv::Mat hist;

	int channels[1] = { 0 };
	int histSize[1] = { 256 };
	float range[2] = { 0.0, 255.0 };

	float* tmp = range;
	auto ranges = const_cast<const float**>(&tmp);

	//CV_EXPORTS void calcHist(const Mat* images, int nimages,
	//	const int* channels, InputArray mask,
	//	OutputArray hist, int dims, const int* histSize,
	//	const float** ranges, bool uniform = true, bool accumulate = false);

	calcHist(&grayImage, 1, channels, cv::Mat(), hist, 1, histSize, ranges);


	double min = 0.0;
	double max = 0.0;

	for (size_t i = 0; i < 256; i++)
	{
		max = hist.at<float>(i) > max ? hist.at<float>(i) : max;
	}

	//cvMinMaxLoc((float*)(hist.data), &min, &max);

	Mat hist_image(cv::Size(256, 500), CV_8U, cv::Scalar(255));
	for (size_t i = 0; i < 256; i++)
	{
        int length = hist.at<float>(i) / max * 500;
        cv::line(hist_image, cv::Point(i, 0), cv::Point(i, length), cv::Scalar::all(0));
	}
	return hist_image;
}

Mat get_binary(Mat& gray_image) 
{
	//ousu 大津阈值
	Mat result;
#ifdef __WINDOWS__
	IplImage gray_in(gray_image);
	IplImage* binary_out = cvCreateImage(cvSize(gray_image.cols, gray_image.rows), IPL_DEPTH_8U, 1);
	double what = cvThreshold(&gray_in, binary_out, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);

	result = cvarrToMat(binary_out);
#endif

	return result;
}

Mat get_edges(Mat& gray_image) 
{
	Mat result;

	morphologyEx(gray_image, result, MORPH_GRADIENT, Mat());

	return result;
}


int fuliye(Mat& input)
{

	int m = getOptimalDFTSize(input.rows);//计算最佳扩充尺寸
	int n = getOptimalDFTSize(input.cols);

	Mat padded;

	copyMakeBorder(input, padded, 0, m - input.rows, 0, n - input.cols, BORDER_CONSTANT, Scalar::all(0));//扩充图像

	Mat planes[]
		= { Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F) };//添加维度，用于存储傅立叶变换的结果
	Mat complexI;
	merge(planes, 2, complexI);//合并通道

	dft(complexI, complexI, DFT_ROWS);//离散傅立叶变换

	split(complexI, planes);//将存储在complexI的结果分解到planes[0],planes[1]中
	magnitude(planes[0], planes[1], planes[0]);//计算幅值

	double max = 0;
	double min = 0;
	


	vector<double> vec_max(complexI.rows);
	for (size_t i = 0; i < complexI.rows; i++)
	{
		Mat line(planes[0], Rect(0, i, complexI.rows, 1));
		minMaxLoc(line, &min, &max);
		vec_max.push_back(max);
		//cout << "max:" << max << "    min:" << min << endl;
	}

	for (size_t i = 0; i < complexI.rows; i++)
	{
		for (size_t j = 0; j < complexI.cols; j++)
		{
			if (planes[0].at<float>(i, j) < vec_max[j])
			{
				complexI.at<Vec2f>(i, j)[0] = 0;
				complexI.at<Vec2f>(i, j)[1] = 0;
			}
		}
	}

	Mat magnitudeImage = planes[0];
	//magnitudeImage = magnitudeImage	+ Scalar::all(1);
	//log(magnitudeImage, magnitudeImage);//用对数表示

	magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols
		& -2, magnitudeImage.rows
		& -2));

	int cx = magnitudeImage.cols / 2;
	int cy = magnitudeImage.rows / 2;
	//Mat q0(magnitudeImage, Rect(0, 0, cx, cy));
	//Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));
	//Mat q2(magnitudeImage, Rect(0, cy, cx, cy));
	//Mat q3(magnitudeImage, Rect(cx, cy, cx, cy));
	//Mat tmp;
	//q0.copyTo(tmp);
	//q3.copyTo(q0);
	//tmp.copyTo(q3);
	//q1.copyTo(tmp);
	//q2.copyTo(q1);
	//tmp.copyTo(q2);

	Mat q0(magnitudeImage, Rect(0, 0, cx, 2*cy));
	Mat q1(magnitudeImage, Rect(cx, 0, cx, 2*cy));

	Mat tmp;
	q0.copyTo(tmp);
	q1.copyTo(q0);
	tmp.copyTo(q1);


	//cout << planes[0] << endl;

	normalize(magnitudeImage, magnitudeImage, 0, 255, NORM_MINMAX);//归一化

	//cout << "-----------------------------------------------\n";

	//cout << planes[0] << endl;

	Mat bdft(magnitudeImage.rows, magnitudeImage.cols, CV_8U);
	for (size_t i = 0; i < magnitudeImage.rows; i++)
	{
		for (size_t j = 0; j < magnitudeImage.cols; j++)
		{
			bdft.at<uchar>(i, j) = magnitudeImage.at<float>(i, j);
		}
	}
	//magnitudeImage.copyTo(bdft);
	Mat bdft_result = get_binary(bdft);
	//cv::imwrite("dft.bmp", magnitudeImage);
	Mat diff_mat = bdft - bdft_result;
	Mat result_image;
	erode(diff_mat, result_image, Mat());
	show_image(result_image);

	Mat invDFT, invDFTcvt;
	idft(complexI, invDFT, DFT_ROWS | DFT_SCALE | DFT_REAL_OUTPUT);//离散傅立叶逆变换
	invDFT.convertTo(invDFTcvt, CV_8U);

	//imwrite("idft.bmp", invDFTcvt);
	show_image(invDFTcvt);

	return 0;
}

int cutout_image(string ifile, string ofile) 
{
	Mat image = imread(ifile, -1);
	int depth = image.channels();
	assert(4 == depth);
	for (size_t i = 0; i < image.rows; i++)
	{
		for (size_t j = 0; j < image.cols; j++) 
		{
			int value = (image.at<Vec4b>(i, j)[0] +	image.at<Vec4b>(i, j)[1] + image.at<Vec4b>(i, j)[2]) / 3;

			image.at<Vec4b>(i, j)[3] = 255 - value;

		}
	}
	imwrite(ofile, image);
	return 0;
}

int load_buf_from_img(string ifile, char *buf, int& width, int& height)
{
    Mat image = imread(ifile, -1);
    if(!image.empty())
    {
        return -1;
    }
    width = image.rows;
    height = image.cols;
    return 0;
}

void show_image(Mat& mat) 
{
#ifdef __WINDOWS__
	shared_ptr<LLWindow> lw(new LLWindow(mat.cols, mat.rows, mat.channels()));
	lw->show_window();
	lw->draw_img((char*)mat.data);
#endif
}

Mat recognise_num(Mat& mat) 
{
	return mat;
}

Mat rectify_image(char * img_path) 
{
	auto image = imread(img_path);
	cv::findChe
	return image;
}

