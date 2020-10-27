#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
    Mat src = imread("C:/Users/user/Desktop/200908/workspace/1.jpg", IMREAD_COLOR);
    Mat src2 = imread("C:/Users/user/Desktop/200908/workspace/2.jpg", IMREAD_COLOR);
    Mat dst(src.rows + 100, src.cols, src.type(), Scalar(255,255,255));
    int yy;
    Mat temp1(src.rows, 1, CV_8UC3);
    Mat temp2(src.rows, 1, CV_8UC3);
    while (true)
    {
        //움직임
        for (int y = 0; y < src.rows; y++)
        {
            temp1.at<Vec3b>(y, 0) = src.at<Vec3b>(y, 0);
            temp2.at<Vec3b>(y, 0) = src.at<Vec3b>(y, src.cols / 2);
            for (int x = 1; x < src.cols; x++)
            {
                src.at<Vec3b>(y, x - 1) = src.at<Vec3b>(y, x);
            }
            src.at<Vec3b>(y, src.cols / 2 - 1) = temp2.at<Vec3b>(y, 0);
            src.at<Vec3b>(y, src.cols - 1) = temp1.at<Vec3b>(y, 0);

            temp1.at<Vec3b>(y, 0) = src2.at<Vec3b>(y, 0);
            temp2.at<Vec3b>(y, 0) = src2.at<Vec3b>(y, src2.cols / 2);
            for (int x = 1; x < src2.cols; x++)
            {
                src2.at<Vec3b>(y, x - 1) = src2.at<Vec3b>(y, x);
            }
            src2.at<Vec3b>(y, src2.cols / 2 - 1) = temp2.at<Vec3b>(y, 0);
            src2.at<Vec3b>(y, src2.cols - 1) = temp1.at<Vec3b>(y, 0);
        }

        for (int y = 0; y < src.rows; y++)
        {
            for (int x = 0; x < src.cols; x++)
            {
                int coord = x - 250;
                yy = sqrt(2500 - coord * coord * 2500 / 62500.0);

                dst.at<Vec3b>(y - yy + 50, x) = src.at<Vec3b>(y, src.cols - x);
                dst.at<Vec3b>(y + yy + 50, x) = src2.at<Vec3b>(y, x);
            }
        }
        imshow("window", dst);
        if (waitKey(50) == 27)
            break;
    }

    return 0;

}