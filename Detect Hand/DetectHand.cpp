#include <opencv.hpp>
#include <imgproc/types_c.h>
#include <vector>
using namespace std;
using namespace cv;


// The function that determines whether the given values are a skin?
bool isSkin(uchar Cr, uchar Cb)
{
	return 140 < Cr && Cr < 170 && 77 < Cb && Cb < 127;
}

// It returns hand's center point by distance transforming.
// also gives radius (hand area)
Point getCenter(const Mat& img, double& radius)
{
	Mat dst;
	int maxIdx[2];
	distanceTransform(img, dst, CV_DIST_L2, 5);
	minMaxIdx(dst, NULL, &radius, NULL, maxIdx, img);
	return Point(maxIdx[1], maxIdx[0]);
}

Mat getMask(const Mat& img)
{
	Mat ycrcb;
	cvtColor(img, ycrcb, COLOR_BGR2YCrCb); // Convert RGB to YCrCb

	vector<Mat> planes;
	split(ycrcb, planes); // split each channel

	Mat mask = Mat::zeros(img.size(), CV_8U); // Create Compatible Matrix
	int nr = img.rows;
	int nc = img.cols;
	for (int i = 0; i < nr; i++)
	{
		uchar* Cr = planes[1].ptr<uchar>(i); // Cr array (one row)
		uchar* Cb = planes[2].ptr<uchar>(i); // Cr array (one row)
		for (int j = 0; j < nc; j++)
		{
			if (isSkin(Cr[j], Cb[j]))
				mask.at<uchar>(i, j) = 255;
		}
	}
	return mask;
}

auto fingerCount(const Mat& mask, Point pt, double radius)
{
	volatile int x, y, finger = 0;
	radius *= 2;
	uchar prev = 1;
	vector<Point> pts;
	for (int i = 1; i < 360; i += 1)
	{
		double radian = i * ((22 / 7.0) / 180);
		x = pt.x + radius * cos(radian);
		y = pt.y - radius * sin(radian);
		
		if (0 <= y && y < mask.rows && 0 <= x && x < mask.cols)
		{ // In mask's area.
			uchar current = mask.at<uchar>(y, x);
			if (!prev && current)
			{
				pts.push_back({ x, y });
				finger++;
			}
			prev = current;
		}

	}
	return make_pair(pts, finger - 1);
}


int main(int argc, char* argv[])
{
	VideoCapture cap;
	if (!cap.open(0))
		return 0;
	while (true)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty()) break;
		Mat mask;// = getMask(frame);
		cvtColor(frame, mask, COLOR_BGR2YCrCb);
		inRange(mask, Scalar(0, 140, 77), Scalar(255, 170, 127), mask);
		//erode(mask, mask, Mat(3, 3, CV_8U, Scalar(1)), Point(-1, -1), 2);

		double radius;;
		auto pt = getCenter(mask, radius);
		circle(frame, pt, radius, { 255,0,0 }, 3);

		auto res = fingerCount(mask, pt, radius);
		for (auto& pt : res.first)
		{
			circle(frame, pt, 1, { 0,0,255 }, 2);
		}
		imshow("img", frame);
		
		switch (res.second)
		{
		case 0:
			cout << "바위" << endl;
			break;
		case 2:
			cout << "가위" << endl;
			break;
		case 5:
			cout << "보" << endl;
			break;
		default:
			cout << "인식 불가" << endl;
			break;
		}

	
		if (waitKey(10) == 27) break;
	}

	
	

}