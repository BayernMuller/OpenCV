#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


// The function that decides the skin.
bool isSkin(uchar Cr, uchar Cb)
{
	return 128 < Cr && Cr < 170 && 73 < Cb && Cb < 158;
}


int main()
{
	Mat img = imread("C:\\Users\\User\\Desktop\\hand.jpg", IMREAD_COLOR);
	cvtColor(img, img, COLOR_BGR2YCrCb); // Convert RGB to YCrCb

	vector<Mat> planes;
	split(img, planes); // split each channel

	Mat mask = Mat::zeros(img.size(), CV_8U); // Create Compatible Matrix
	int nr = img.rows;
	int nc = img.cols;

	for (int i = 0; i < nr; i++) 
	{
		uchar* Cr = planes[1].ptr<uchar>(i); // Cr array (one row)
		uchar* Cb = planes[2].ptr<uchar>(i); // Cr array (one row)
		for (int j = 0; j < nc; j++) 
		{
			mask.at<uchar>(i, j) = isSkin(Cr[j], Cb[j]) ? 255 : 0; 
			// if it is a skin, set pixel white. otherwise black.
		}
	}
	
	imshow("img", mask);
	waitKey();
}
