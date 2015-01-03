#include "core.hpp"
#include "imgcodecs.h"
#include "highgui.h"

using namespace zl;


int main()
{
	
	Mat image;
	imread(image, "D:\\Dev\\vs_projects\\OpenZL\\cache\\test.jpg", 1);
	imshow("Test", image);

	Mat gray;
	cvtColor(image, gray, ZL_RGB2GRAY);
	imshow("Gray", gray);
	//bool ret = imwrite("D:\\Dev\\vs_projects\\OpenZL\\cache\\test_out.jpg", image, 1, 90);
	//for (int i = 1; i < 10; i++)
	//{
	//	char title[20];
	//	sprintf(title, "Debug_%d", i);
	//	imshow(title, image);
	//}

	destroy_window("Debug_5");

	waitkey();

	destroy_all_windows();
	

	zl::hold_screen();
	return 0;
}

