#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) 
{
	Mat a = imread("C:\\Users\\User\\Desktop\\motown.jpg");
	Mat b = imread("C:\\Users\\User\\Desktop\\countinmyguap.jpg");
	
	vector<Point2f> corners(4);
	vector<Point2f> warpCorners(4);
	warpCorners[0] = corners[1] = Point2f(0, 0);
	warpCorners[1] = corners[0] = Point2f(a.cols, 0);
	warpCorners[2] = corners[3] = Point2f(0, a.rows);
	warpCorners[3] = corners[2] = Point2f(a.cols, a.rows);

	Size warpSize(a.cols, a.rows);
	Mat warpImg(warpSize, a.type());
	Mat trans = getPerspectiveTransform(corners, warpCorners);
	warpPerspective(a, warpImg, trans, warpSize);

	for (int i = 0; i <= a.cols; i+=5)
	{
		Mat img;
		b.copyTo(img);
		for (int j = 0; j < a.cols - i; j++)
		{
			for (int k = 0; k < a.rows; k++)
			{
				img.at<Vec3b>(k, j) = a.at<Vec3b>(k, j);
			}
		}

		int cnt = 0;
		for (int j = a.cols - i - i; j < a.cols - i; j++)
		{
			for (int k = 0; k < a.rows; k++)
			{
				if (j < 0)
					continue;
				img.at<Vec3b>(k, j) = warpImg.at<Vec3b>(k, cnt);
			}
			cnt++;
		}
		imshow("result", img);
		if (waitKey(20) == 27)
			return 0;
	}

	waitKey(0);
	return 0;
}
