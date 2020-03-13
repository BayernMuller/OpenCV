#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace std;
using namespace cv;


// The function that determines whether the given values are a skin?
bool isSkin(uchar Cr, uchar Cb)
{
	return 128 < Cr && Cr < 170 && 73 < Cb && Cb < 158;
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

int fingerCount(const Mat& mask, Point pt, double radius)
{
	int cx, cy, finger = 0;
	radius *= 2;
	uchar prev = 1;
	for (int i = 1; i < 360; i += 3)
	{
		double radian = i * ((22 / 7.0) / 180);
		cx = radius * cos(radian);
		cy = radius * sin(radian);
		if (pt.y - cy < mask.cols && pt.x + cx < mask.rows)
		{ // In mask's area.
			uchar current = mask.at<uchar>(pt.y - cy, pt.x + cx);
			if (!prev && current)
			{
				finger++;
			}
			prev = current;
		}
	}
	return finger - 1;
}


int main()
{
	Mat img = imread("C:\\Users\\User\\Desktop\\hands\\rock.jpg", IMREAD_COLOR);
	Mat mask = getMask(img);
	erode(mask, mask, Mat(3, 3, CV_8U, Scalar(1)), Point(-1, -1), 2);

	imshow("ilbe", mask);
	waitKey();

	double radius;
	auto pt = getCenter(mask, radius);
	circle(img, pt, radius, { 255,0,0 }, 3);

	cout << fingerCount(mask, pt, radius) << endl;
	
	imshow("img", img);
	waitKey();
}
