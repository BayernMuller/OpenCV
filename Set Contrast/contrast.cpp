#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("res//test.png", IMREAD_COLOR);
	Mat new_image = Mat::zeros(img.size(), img.type());
	medianBlur(img, img, 11);
	constexpr int beta = 100;
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			const auto& ori = img.at<Vec3b>(y, x);
			auto& pixel = new_image.at<Vec3b>(y, x);
			for (int c = 0; c < 3; c++)
			{	
				if (ori[c] > 128)
					pixel[c] = (ori[c] + beta > 255) ? 255 : ori[c] + beta;
				else
					pixel[c] = (ori[c] - beta < 0) ? 0 : ori[c] - beta;
			}
		}
	}
}