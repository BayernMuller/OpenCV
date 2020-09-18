#include <opencv.hpp>
#include <imgproc/types_c.h>

using namespace cv;
using namespace std;

#define WIN_SIZE 800

int index = 0;
Point pt[3];
Mat img(WIN_SIZE, WIN_SIZE, CV_8UC3, Scalar(255, 255, 255));
Scalar colors[3] = { {255,0,0}, {0,255,0}, {0,0,255} };

void onMouse(int, int, int, int, void*);
void DrawFunction();
void calculate(double num[]);

int main()
{
	namedWindow("coordinate");
	setMouseCallback("coordinate", onMouse, 0);
	imshow("coordinate", img);
	waitKey();
}

void calculate(double num[])
{
	double a = pt[0].x, b = pt[0].y;
	double c = pt[1].x, d = pt[1].y;
	double f = pt[2].x, g = pt[2].y;
	double A = ((g * (a * a - c * c)) + (d * (f * f - a * a)) + (b * (c * c - f * f))) / (2 * ((g - d) * a + (b - g) * c + (d - b) * f));
	double C = (b - d) / ((a * a - c * c) + (2 * A) * (c - a));
	double B = d - (C * ((c - A) * (c - A)));
	num[0] = C;
	num[1] = A;
	num[2] = B;
}

void DrawFunction()
{
	double num[3];
	calculate(num);
	for (int i = 0; i < 3; i++)
	{
		cout << num[i] << endl;
	}

	Point prev, current;
	for (int i = 0; i < WIN_SIZE; i++)
	{
		int y = num[0] * (i - num[1]) * (i - num[1]) + num[2];
		y = max(min(WIN_SIZE - 1, y), 0);
		current = Point(i, y);
		if (prev.y != 0 && prev.y != WIN_SIZE - 1)
			line(img, prev, current, Scalar(0,0,0), 2);
		prev = current;
	}
	
	imshow("coordinate", img);
	
}

void onMouse(int ev, int x, int y, int flags, void* params)
{
	if (ev != EVENT_LBUTTONDOWN)
		return;
	if (index == 3)
	{
		index = 0;
		img = Scalar(255, 255, 255);
		imshow("coordinate", img);
	}
	
	cout << index << endl;
	pt[index].x = x;
	pt[index].y = y;
	circle(img, pt[index], 5, colors[index],3);
	imshow("coordinate", img);
	if (index == 2)
	{
		DrawFunction();
		cout << "Draw!" << endl;
	}
	index++;
}