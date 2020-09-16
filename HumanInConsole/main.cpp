#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

const char* table[] =
{
	"  ", "..", ",,", "--", "++", "**", "==", "//" ,"&&", "##", "@@", "WW"
};

void draw(Mat& img)
{
	int detail = 13;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	if (img.empty())
		return;
	for (int i = 0; i < img.rows; i += detail)
	{	
		for (int j = 0; j < img.cols; j += detail)
		{
			auto pt = img.at<Vec3b>(i, j);
			
			int average = (pt[0]+ pt[1] + pt[2]) / 3.0;
			printf("%s", table[average * 11 / 255]);
		}
		printf("\n");
	}
}

int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;
	system("cls");
	namedWindow("window");
	Mat image;
	while (waitKey(20) != 27)
	{
		cap >> image;
		if (image.empty())
			return -1;
		imshow("window", image);
		draw(image);
	}
	return 0;
}