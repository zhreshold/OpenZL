#include "core.hpp"
#include "imgcodecs.h"
#include "highgui.h"

using namespace zl;


int main()
{
	//println("q is:", (int)('q'));
	//int key = -1;
	//while (key != 'q')
	//{
	//	key = waitkey(1);
	//	if (key != -1)
	//	{
	//		println(key);
	//	}
	//}

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

	draw_line(image, Point(100, 100), Point(500, 100), Scalar(255, 255, 0), 20);
	draw_line(image, Point(700, 100), Point(700, 500), Scalar(255, 255, 0), 20);
	Timer te;
	draw_line(image, Point(700, 100), Point(700, 500), c, 1);
	println("Line1: ", te.get_elapsed_time_us(), "us");
	te.update();
	draw_line(image, Point(300, 500), Point(20,20), c, 3);
	println("Line2: ", te.get_elapsed_time_us(), "us");
	te.update();
	draw_circle(image, 600, 600, 100, c, 10);
	println("circle: ", te.get_elapsed_time_us(), "us");
	te.update();
	draw_rectangle(image, 500, 400, 300, 300, c, 20, 0);
	println("Rect: ", te.get_elapsed_time_us(), "us");
	te.update();
	draw_rectangle(image, 500, 400, 300, 300, Scalar(255, 255, 0), 2, 0);
	println("Rect2: ", te.get_elapsed_time_us(), "us");

	Vecpt pts;
	pts.push_back(Point(30, 40));
	pts.push_back(Point(50, 5));
	pts.push_back(Point(100, 60));
	pts.push_back(Point(200, 400));
	pts.push_back(Point(50, 70));

	draw_polygon(image, pts, c);



	draw_circle(image2, 600, 600, 500, Scalar(255, 255, 0), 1, true);

	Point pt1 = Point(200, 100) + Point(300, 100);

	//imshow("Vec2 image", v);
	imshow("drawn", image);
	//imshow("drawn2", image2);
	
	println("Elasped time: ", t.get_elapsed_time_ms(), " ms");

	t.update();
	waitkey(10000);
	println("Waited: ", t.get_elapsed_time_ms(), " ms");

	zl::hold_screen();
	return 0;
}

