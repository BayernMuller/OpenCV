#include "opencv2/core/core.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

void indexing(vector<Vec3b>& info)
{
	for (int i = 0; i <= 509; i++)
	{
		char str[64] = { 0, };
		Vec3b avr = 0;
		sprintf_s(str, "images/dbs%04d.jpg", i);
		Mat img = imread(str);
		
		for (int x = 0; x < img.rows; x++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					if (x + j == 0)
					{
						avr[k] = img.at<Vec3b>(x, j)[k];
						continue;
					}
					avr[k] = (avr[k] + img.at<Vec3b>(x, j)[k]) / 2.0;
				}
			}
		}

		info.push_back(avr);
	}
}

int main(int argc, char** argv)
{
	vector<Vec3b> info;
	indexing(info);
	Mat img = imread("C:\\Users\\User\\Desktop\\abc.jpg");
	for (int i = 0; i < img.rows - 32; i += 32)
	{
		for (int j = 0; j < img.cols - 32; j++)
		{

		}
	}
	imshow("dasd", mat);
	waitKey();
	return 0;
}
