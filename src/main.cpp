#include "core.hpp"
#include "imgcodecs.h"

using namespace zl;


int main()
{
	
	Mat image;
	imread(image, "D:\\Dev\\vs_projects\\OpenZL\\cache\\test.png", 1);

	bool ret = imwrite("D:\\Dev\\vs_projects\\OpenZL\\cache\\test_out.jpg", image, 1, 90);
	

	zl::hold_screen();
	return 0;
}