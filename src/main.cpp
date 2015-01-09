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
	v2 = image;

	gray2 = gray;

	imshow("Vec2 image", v);
	
	println("Elasped time: ", t.get_elapsed_time_ms(), " ms");

	t.update();
	waitkey(10000);
	println("Waited: ", t.get_elapsed_time_ms(), " ms");

	zl::hold_screen();
	return 0;
}

