#include "opencv2/core/core.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;

Mat dftOfOriginal;

void takeDFT(const Mat& src, Mat& dst);
void invertDFT(const Mat& src, Mat& dst);
void showDFT(const Mat& src);
void recenterDFT(Mat& src);

void mouse(int event, int x, int y, int flags, void* arg)
{
	if (event != EVENT_LBUTTONDOWN)
		return;

	recenterDFT(dftOfOriginal);
	circle(dftOfOriginal, Point(x, y), 10, 0, -1);
	recenterDFT(dftOfOriginal);
	showDFT(dftOfOriginal);

	Mat invertedDFT;
	invertDFT(dftOfOriginal, invertedDFT);
	imshow("inv", invertedDFT);
}

int main(int argc, char** argv)
{
	Mat img = imread("C:/Users/User/Desktop/4.png", IMREAD_GRAYSCALE);
	if (img.empty())
		return -1;
	
	namedWindow("img");
	namedWindow("inv");
	namedWindow("dft");
	imshow("img", img);

	Mat imgFloat;
	img.convertTo(imgFloat, CV_32FC1, 1 / 255.0);
	takeDFT(imgFloat, dftOfOriginal);
	showDFT(dftOfOriginal);

	setMouseCallback("dft", mouse, &dftOfOriginal);
	

	waitKey();
	return 0;
}

void takeDFT(const Mat& src, Mat& dst)
{
	Mat originalComplex[2] = { src, Mat::zeros(src.size(), CV_32F) };
	Mat dftReady;
	merge(originalComplex, 2, dftReady);
	Mat dftOfOriginal;
	dft(dftReady, dftOfOriginal, DFT_COMPLEX_OUTPUT);
	dst = dftOfOriginal;
}

void showDFT(const Mat& src)
{
	Mat splitArray[2] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	split(src, splitArray);
	Mat dftMagnitude;
	magnitude(splitArray[0], splitArray[1], dftMagnitude);
	dftMagnitude += Scalar::all(1);
	log(dftMagnitude, dftMagnitude);
	normalize(dftMagnitude, dftMagnitude, 0, 1, 32);
	recenterDFT(dftMagnitude);
	imshow("dft", dftMagnitude);
}

void invertDFT(const Mat& src, Mat& dst)
{
	Mat inverse;
	dft(src, inverse, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	dst = inverse;
}

void recenterDFT(Mat& src)
{
	int x = src.cols / 2;
	int y = src.rows / 2;

	Mat q1(src, Rect(0, 0, x, y));
	Mat q2(src, Rect(x, 0, x, y));
	Mat q3(src, Rect(0, y, x, y));
	Mat q4(src, Rect(x, y, x, y));

	Mat swapMap;
	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q4);

	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);
}