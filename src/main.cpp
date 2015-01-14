#include "core.hpp"
#include "imgcodecs.h"
#include "highgui.h"

using namespace zl;


int main()
{
	Timer t;
	Mat image;
	imread(image, "D:\\Dev\\vs_projects\\OpenZL\\cache\\test.png", 1);
	imshow("Test", image);

	Mat empty;
	Mat imageTest = empty;

	Mat gray;
	cvt_color(image, gray, ZL_RGBA2GRAY);

	imshow("Gray", gray);
	//bool ret = imwrite("D:\\Dev\\vs_projects\\OpenZL\\cache\\test_out.jpg", image, 1, 90);
	//for (int i = 1; i < 10; i++)
	//{
	//	char title[20];
	//	sprintf(title, "Debug_%d", i);
	//	imshow(title, image);
	//}

	//destroy_window("Debug_5");

	//waitkey();

	//destroy_all_windows();

	Mat gray2;

	Vec2u8 v, v2;

	v = gray;

	gray2 = gray;
	

	Scalar c = Scalar(255, 0, 0);

	Mat image2 = image;

	draw_line(image, Point(20, 500), Point(500,20), c, 5);
	draw_circle(image, 600, 600, 100, c, 10);
	draw_circle(image2, 600, 600, 500, c, 1);

	Point pt1 = Point(200, 100) + Point(300, 100);

	//imshow("Vec2 image", v);
	imshow("drawn", image);
	imshow("drawn2", image2);
	
	println("Elasped time: ", t.get_elapsed_time_ms(), " ms");

	t.update();
	waitkey(10000);
	println("Waited: ", t.get_elapsed_time_ms(), " ms");

	zl::hold_screen();
	return 0;
}

